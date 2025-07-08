#include <cstdio>
#include <vector>
#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"
#include "resources/generated/map/map_01.h"
#include "resources/generated/map/map_01_fg.h"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/tileset/tileset_player.h"
#include "resources/generated/tileset/tileset_blue.h"
#include "resources/generated/talkscript/test_talkscript.h"
#include "resources/generated/font/k8x12.h"
#include "resources/generated/btree/test_btree.h"

namespace {

using mgc::drivers::platform::display::ST7789;
using mgc::drivers::platform::sound::MmlPsgSoundController;
using mgc::camera::SimpleCameraFollower;
using mgc::collision::CollisionDetectorBoxToMap;
using mgc::collision::CollisionDetectorBoxToBox;
using mgc::collision::Hitbox;
using mgc::collision::HitboxOffset;
using mgc::collision::HitboxSize;
using CornerPushDirection = mgc::collision::CollisionDetectorBoxToMap::CornerPushDirection;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;;
using mgc::graphics::AsyncDoubleFramebuffer;
using mgc::platform::input::Key;
using mgc::control::talkflow::DefaultTalkflowController;
using mgc::control::talkflow::DialogueboxConfig;
using mgc::control::talkflow::SelectboxConfig;

ST7789 display_driver;
MmlPsgSoundController sound_controller;
auto& gamepad = mgc::drivers::platform::input::default_gamepad();

DefaultTalkflowController talkflow_controller(gamepad);
mgc::control::btree::BTreeController<mgc::drivers::platform::timer::FreeRunningTimerU32> btc(gamepad);

SimpleCameraFollower camera;

mgc_color_t buf1[240*240];
mgc_color_t buf2[240*240];

Framebuffer fb(buf1, 240, 240);
AsyncDoubleFramebuffer dfb(buf1, buf2, 240, 240);
CellBuffer cell_buffer;
mgc::render::CellRenderer<ST7789> cell_renderer(cell_buffer, display_driver, &camera);
mgc::render::Renderer<ST7789> renderer(fb, display_driver, &camera);
mgc::render::DoubleBufferedRenderer<ST7789> dfb_renderer(dfb, display_driver, &camera);

size_t npc1_talk_count = 0;

// Rendering logic depending on buffer mode
// - UseCellBuffer: simple cell-based rendering using 16x16 pixel grid.
// - UseFrameBuffer: single framebuffer rendering (blocking)
// - UseDoubleBuffer: double buffering with asynchronous rendering
enum class RenderType {
    UseCellBuffer,
    UseFrameBuffer,
    UseDoubleBuffer
};

auto render_type = RenderType::UseDoubleBuffer;

struct Stage1 : mgc::entities::TilemapImpl<Stage1> {
    Stage1() {
        tilegrid_.bind_listener(listener_);
        tilegrid_.set_tileset(tileset_map_elements);
        tilegrid_.set_tile_id_map(map_01);
        tilegrid_.set_position(mgc::math::Vec2i(0, 0));
        this->set_collision_map_impl(map_01);
    }

    void update() {
        listener_.update();
    }

private:
    struct Listener : mgc::parts::IBasicTilegridListener {
        uint8_t on_request_tile_id(uint8_t tile_id, uint16_t row, uint16_t col) override {
            if ( tile_id == 20 ) {
                // Flame flickering effect for tile_id 20 (used in tilemap listener)
                tile_id += counter%3;
            }
            return tile_id;
        }
        void update() {
            counter++;
        }
    private:
        int counter = 0;
    } listener_;
};


struct Stage1_FG : mgc::entities::TilemapImpl<Stage1_FG> {
    Stage1_FG() {
        tilegrid_.set_tileset(tileset_map_elements);
        tilegrid_.set_tile_id_map(map_01_fg);
        tilegrid_.set_position(mgc::math::Vec2i(0, 0));
        tilegrid_.set_parallax_factor(mgc::graphics::ParallaxFactor(2.0, 1.0));
    }
};


struct NPC1 : mgc::entities::ActorImpl<NPC1, 1> {

    NPC1() {
        sprite_.set_tileset(tileset_blue);
        sprite_.set_tile_index(1);
        // Setup basic hitbox for NPC1
        hitboxes_[0].offset = mgc::collision::HitboxOffset(0, 0);
        hitboxes_[0].size = mgc::collision::HitboxSize(16, 16);
        hitboxes_[0].enabled = true;

        this->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(11), MGC_CELL2PIXEL(38)));
    }
    ~NPC1() = default;
};


struct Player : mgc::entities::ActorImpl<Player, 1> {

    Player() : vy_(0), jumping_(false) {
        sprite_.set_tileset(tileset_player);
        sprite_.set_tile_index(3);
        hitboxes_[0].offset = mgc::collision::HitboxOffset(0, 0);
        hitboxes_[0].size = mgc::collision::HitboxSize(16, 16);
        hitboxes_[0].enabled = true;

        this->set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(38)));
    }
    ~Player() = default;

    void update() {
        // TODO update animation
        auto pos = this->position();
        if ( gamepad.is_pressed(Key::Left) ) {
             pos.x -= 4;
        } else if ( gamepad.is_pressed(Key::Right) ) {
             pos.x += 4;
        } else { }
        
        // Jump logic with simple vertical velocity (vy_)
        if ( gamepad.just_pressed(Key::Cancel) ) {
            if ( !jumping_ ) {
                jumping_ = true;
                vy_ = -12;
            }
        }
        
        pos.y += vy_;
        if ( vy_ < MGC_CELL_LEN ) {
            vy_++;
        }

        this->set_position(pos);

        if ( gamepad.just_pressed(Key::Control) ) {
            sound_controller.play_sound_effect(0, 0.0); // Meow
        }
    }

    template <typename Other>
    void on_hit_box_to_box_impl(
            const Other& other,
            const mgc::collision::BoxCollisionInfo& info
    ) {
        // Talkflow interaction: trigger only when near NPC and player releases Enter key
        if constexpr (std::is_same_v<Other, NPC1>) {
            if ( !talkflow_controller.in_progress() ) {
                if ( gamepad.just_released(Key::Enter) ) {
                    npc1_talk_count++;
                    talkflow_controller.begin(MGC_TEST_TALKSCRIPT_START);
                }
            }
        }
    }

    template <typename ObjT, typename MapT>
    void handle_map_pushback_result_impl(
            const ObjT& obj,
            const MapT& map,
            const mgc::collision::MapPushbackInfo& info
    ) { 
        if constexpr (std::is_same_v<MapT, Stage1>) {

            auto pos = sprite_.position();
            // Pushback response logic: stop falling or bounce depending on direction
            if ( info.pushback.y < 0 ) {
                vy_ = 0;
                pos.y += info.pushback.y;
                jumping_ = false;
            } else if ( info.pushback.y > 0 ) {
                vy_ *= -1;
                pos.y += info.pushback.y;
            } else { }
            pos.x += info.pushback.x;

            sprite_.set_position(pos);
        }
    }

private:
    int16_t vy_;
    bool jumping_;
};



const mgc_mml_record_t se_records[1] = {
    // Meow
    {
        .id = 0,
        .mml = "$P-60$B60L4O5F#"
    }, 
};

const mgc_mml_record_t bgm_records[1] = {
    // The Flea Waltz
    { 
        .id = 0,
        .mml = 
            // CH A
            "[0"
                "T140"
                "$P-30$B60"
                "$E1$A0$H40$D100$S80$F2000"
                "[2 L4O5 D#8C#8<F#>F#F#]"
                "L4O5 D#8C#8<F#>F#<D#>F#<C#>FF"
                "[2 L4O5 D#8C#8<C#>FF]"
                "L4O5 D#8C#8<C#>F<D#>F<F#>F#F#"

                "[2 L4O5 D#8C#8A#F#F#]"
                "L4O5 D#8C#8A#F#>C#<F#>D#<FF"
                "[2 L4O5 D#8C#8>D#<FF]"
                "L4O5 D#8C#8>D#<F>C#<FA#F#F#"

                "L4O5 D#8C#8<F#>F#<C#>F#<F#>F#<C#>F# <F#FF#GG# >FF"
                "L4O5 D#8C#8<G#>F <D#>F <G#>F <D#>F  <G#GG#AA# >F#F#"

                "[2 L4O5 D#8C#8<F#>F#F#]"
                "L4O5 D#8C#8<F#>F#<D#>F#<C#>FF"
                "[2 L4O5 D#8C#8<C#>FF]"
                "L4O5 D#8C#8<C#>F<D#>F<F#>F#F#"
            "]"
            ","
            // CH B
            "[0"
                "T140V15"
                "$P-720$B60I0"
                "R4"
                "O2[32 V15F#8 V13H64R16.. V15F#8 V13H64R16.. V15F#8 V13H64R16.. | V15F#8 V13H64R16..]"
            "]"
            ","
            // CH C 
            "[0"
                "T140"
                "$P-30$B60"
                "$E1$A0$H40$D100$S80$F2000"
                "[2"
                    "[2 L4O4 R2 A#A#]"
                    "L4O4 R2 A# R A# R BB"
                    "[2 L4O4 R2 BB]"
                    "L4O4 R2 B  R B  R A#A#"
                "]"
                "L4O4 R2 A# R A# R A# R A# R1 R B  B"
                "L4O4 R2 B  R B  R B  R B  R1 R A# A#"

                "[2 L4O4 R2 A#A#]"
                "L4O4 R2 A# R A# R BB"
                "[2 L4O4 R2 BB]"
                "L4O4 R2 B  R B  R A#A#"
            "]"
    }
};


}

static char label_buf_1[32] = {"---------------"};
static char label_buf_2[32] = {"---------------"};
static char label_buf_3[32] = {"---------------"};
static char label_buf_4[32] = {"---------------"};
static char label_buf_5[32] = {"---------------"};

int main() {

    float sound_speed_factor = 1.0f;

	auto now = mgc::drivers::platform::timer::FreeRunningTimerU32::now_ms(); 

    display_driver.init(50*1000*1000); // over clock

    sound_controller.init();
    sound_controller.set_background_music_list(bgm_records, countof(bgm_records));
    sound_controller.set_sound_effect_list(se_records, countof(se_records));
    sound_controller.set_lpf_enabled(true);
    sound_controller.set_master_volume(0.5);
    sound_controller.play_background_music(0, 0.0);

    gamepad.init(); 

    mgc::parts::BasicLabel label1;
    label1.set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(0)));
    label1.set_font(k8x12);
    label1.set_text(label_buf_1);
    label1.set_size(mgc::parts::types::Size(8*sizeof(label_buf_1), 12));

    mgc::parts::BasicLabel label2;
    label2.set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(1)));
    label2.set_font(k8x12);
    label2.set_text(label_buf_2);
    label2.set_size(mgc::parts::types::Size(8*sizeof(label_buf_2), 12));

    mgc::parts::BasicLabel label3;
    label3.set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(2)));
    label3.set_font(k8x12);
    label3.set_text(label_buf_3);
    label3.set_size(mgc::parts::types::Size(8*sizeof(label_buf_3), 12));

    mgc::parts::BasicLabel label4;
    label4.set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(3)));
    label4.set_font(k8x12);
    label4.set_text(label_buf_4);
    label4.set_size(mgc::parts::types::Size(8*sizeof(label_buf_4), 12));

    mgc::parts::BasicLabel label5;
    label5.set_position(mgc::math::Vec2i(MGC_CELL2PIXEL(2), MGC_CELL2PIXEL(4)));
    label5.set_font(k8x12);
    label5.set_text(label_buf_5);
    label5.set_size(mgc::parts::types::Size(8*sizeof(label_buf_5), 12));


    Stage1 stage1;
    Stage1_FG stage1_fg;
    Player player;
    NPC1 npc1;
    CollisionDetectorBoxToMap collision_detector;

    talkflow_controller.set_font(k8x12);
    talkflow_controller.set_talkscript(test_talkscript);
    struct TalkflowListener : mgc::control::talkflow::ITalkflowListener {
        bool on_decision(mgc_node_idx_t tag) override {
            if ( tag == MGC_TEST_TALKSCRIPT_TAG_DECISION__IS_FIRST_TALK ) {
               return ( npc1_talk_count == 1 );
            } else {
                return true; 
            }
        }
    };
    TalkflowListener listener;
    talkflow_controller.bind_listener(listener);

    talkflow_controller.set_dialoguebox_config(
        DialogueboxConfig {
            mgc::math::Vec2i(8, MGC_CELL2PIXEL(6)),
            mgc::parts::types::Size(150, 40),
            2,1,4,2
        }
    );
    talkflow_controller.set_selectbox_config(
        SelectboxConfig {
            mgc::math::Vec2i(100, MGC_CELL2PIXEL(6) + 8),
            mgc::parts::types::Size(80, 80),
            "*",
            mgc::math::Vec2i(10, 0)
        }
    );


    struct Listener : mgc::control::btree::IBTreeListener<mgc::control::btree::BTreeController<mgc::drivers::platform::timer::FreeRunningTimerU32>> {
        LeafResult on_proc_leaf(std::string_view id, const DurationT& duration, mgc_btree_tag_t tag) override {
            
            ::snprintf(label_buf_1, sizeof(label_buf_1)-1, "leaf_id: %s", id.data());
            ::snprintf(label_buf_2, sizeof(label_buf_2)-1, "input: %08u", duration.input_idle_time);
            ::snprintf(label_buf_3, sizeof(label_buf_3)-1, "tree: %08u", duration.tree_elapsed);
            ::snprintf(label_buf_4, sizeof(label_buf_4)-1, "composite: %08u", duration.composite_elapsed);
            ::snprintf(label_buf_5, sizeof(label_buf_5)-1, "leaf: %08u", duration.leaf_elapsed);

            if ( id == "cond/timer/over_60s" ) {
                if ( duration.input_idle_time > 60'000 ) {
                    return LeafResult::Success;
                } else {
                    return LeafResult::Failure;
                }
            } else if ( id == "action/sleep" ) {
                return LeafResult::Success;

            } else if ( id == "cond/timer/over_30s" ) {

                if ( duration.input_idle_time > 30'000 ) {
                    return LeafResult::Success;
                } else {
                    return LeafResult::Failure;
                }

            } else if ( id == "action/dance" ) {
                
                if ( duration.leaf_elapsed <= 10'000 ) {
                    return LeafResult::Running;
                } else {
                    return LeafResult::Success;
                }

            } else if ( id == "idle_breathing" ) {
                return LeafResult::Success;

            } else { }
            
            return LeafResult::Running;
        }
    } bt_listener;

    btc.set_btree(test_btree);
    btc.bind_listener(bt_listener);


    std::vector<const mgc::features::Drawable*> drawables;
    drawables.push_back(&stage1.tilegrid());
    drawables.push_back(&player.sprite());
    drawables.push_back(&npc1.sprite());
    drawables.push_back(&stage1_fg.tilegrid());
    drawables.push_back(&talkflow_controller);
    drawables.push_back(&label1);
    drawables.push_back(&label2);
    drawables.push_back(&label3);
    drawables.push_back(&label4);
    drawables.push_back(&label5);

    std::vector<const mgc::features::CellDrawable*> cell_drawables;
    cell_drawables.push_back(&stage1.tilegrid());
    cell_drawables.push_back(&player.sprite());
    cell_drawables.push_back(&npc1.sprite());
    cell_drawables.push_back(&stage1_fg.tilegrid());
    cell_drawables.push_back(&talkflow_controller);
    cell_drawables.push_back(&label1);
    cell_drawables.push_back(&label2);
    cell_drawables.push_back(&label3);
    cell_drawables.push_back(&label4);
    cell_drawables.push_back(&label5);


    camera.set_x_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(3));
    camera.set_x_follow_enabled(true);
    camera.set_y_follow_enabled(true);
    camera.set_target(player.sprite());

    renderer.set_back_color(MGC_COLOR_BLACK);

    while (1) {

        gamepad.proc();
        // Adjust sound playback speed with pitch correction
        // Press Menu to speed up, Home to slow down
        if ( gamepad.just_pressed(Key::Menu) ) {
            sound_speed_factor += 0.01f;
            int16_t pitch_factor = (int16_t)(360 * (sound_speed_factor - 1.0f));
            sound_controller.set_speed_factor(sound_speed_factor);
            sound_controller.shift_pitch_by_degree(pitch_factor);
        } else if ( gamepad.just_pressed(Key::Home) ) {
            sound_speed_factor -= 0.01f;
            int16_t pitch_factor = (int16_t)(360 * (sound_speed_factor - 1.0f));
            sound_controller.set_speed_factor(sound_speed_factor);
            sound_controller.shift_pitch_by_degree(pitch_factor);
        } else { }


        stage1.update();

        if ( btc.has_finished() ) {
            btc.reset_state();
        }
        btc.proc();

        talkflow_controller.proc();
        // If a talkflow is active, player update and collisions are paused
        if ( talkflow_controller.in_progress() ) {
            // Do nothing - wait for talk to finish
        } else if ( talkflow_controller.has_finished() ) {
            talkflow_controller.reset_state();
        } else {
            player.update();
            collision_detector.detect(player, stage1, CornerPushDirection::PreferX);
            CollisionDetectorBoxToBox::detect(player, npc1);
        }

        camera.update_follow_position();

        if ( render_type == RenderType::UseCellBuffer ) {

            // If this function is called after an asynchronous draw,
            // it must wait until display_driver.is_busy() returns false.
            cell_renderer.draw_all_cells_and_transfer(cell_drawables.data(), cell_drawables.size());

        } else if ( render_type == RenderType::UseFrameBuffer ) {

            // Same as when using cell_buffer.
            renderer.draw_to_framebuffer(drawables.data(), drawables.size());
            renderer.transfer_to_display_blocking();

        } else if ( render_type == RenderType::UseDoubleBuffer ) {

            dfb_renderer.draw_to_framebuffer(drawables.data(), drawables.size());

            dfb_renderer.wait_until_idle_interrupt();

            dfb_renderer.transfer_to_display_async();

        } else { }
    }


    return 0;
}
