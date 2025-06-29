#include <vector>
#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "resources/generated/map/map_01.h"
#include "resources/generated/map/map_01_fg.h"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/tileset/tileset_player.h"
#include "resources/generated/tileset/tileset_blue.h"
#include "resources/generated/talkscript/test_talkscript.h"
#include "resources/generated/font/k8x12.h"

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
using mgc::platform::input::Key;
using mgc::control::talkflow::DefaultTalkflowController;
using mgc::control::talkflow::DialogueboxConfig;
using mgc::control::talkflow::SelectboxConfig;

ST7789 display_driver;
MmlPsgSoundController sound_controller;
auto& gamepad = mgc::drivers::platform::input::default_gamepad();

DefaultTalkflowController talkflow_controller(gamepad);

SimpleCameraFollower camera;
mgc::render::Renderer<ST7789> renderer(display_driver, &camera);

mgc_color_t buf1[240*240];
mgc_color_t buf2[240*240];
std::array<Framebuffer, 2> fb_array = {
    Framebuffer(buf1, 240, 240),
    Framebuffer(buf2, 240, 240)
};
size_t fb_index = 0;

CellBuffer cell_buffer;

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


int main() {

    float sound_speed_factor = 1.0f;

    display_driver.init(50*1000*1000); // over clock

    sound_controller.init();
    sound_controller.set_background_music_list(bgm_records, countof(bgm_records));
    sound_controller.set_sound_effect_list(se_records, countof(se_records));
    sound_controller.set_lpf_enabled(true);
    sound_controller.set_master_volume(0.5);
    sound_controller.play_background_music(0, 0.0);

    gamepad.init(); 

    Stage1 stage1;
    Stage1_FG stage1_fg;
    Player player;
    NPC1 npc1;
    CollisionDetectorBoxToMap collision_detector;

    talkflow_controller.set_font(k8x12);
    talkflow_controller.set_talkscript(test_talkscript);
    //struct TalkflowListener : mgc::control::talkflow::ITalkflowListener { };
    //TalkflowListener listener;
    //talkflow_controller.bind_listener(listener);

    talkflow_controller.set_dialoguebox_config(
        DialogueboxConfig {
            mgc::math::Vec2i(8, 8),
            mgc::parts::types::Size(150, 40),
            2,1,4,2
        }
    );
    talkflow_controller.set_selectbox_config(
        SelectboxConfig {
            mgc::math::Vec2i(100, 16),
            mgc::parts::types::Size(80, 80),
            "*",
            mgc::math::Vec2i(10, 0)
        }
    );



    std::vector<const mgc::features::Drawable*> drawables;
    drawables.push_back(&stage1.tilegrid());
    drawables.push_back(&player.sprite());
    drawables.push_back(&npc1.sprite());
    drawables.push_back(&stage1_fg.tilegrid());
    drawables.push_back(&talkflow_controller);

    std::vector<const mgc::features::CellDrawable*> cell_drawables;
    cell_drawables.push_back(&stage1.tilegrid());
    cell_drawables.push_back(&player.sprite());
    cell_drawables.push_back(&npc1.sprite());
    cell_drawables.push_back(&stage1_fg.tilegrid());
    cell_drawables.push_back(&talkflow_controller);


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
            renderer.render(cell_buffer, cell_drawables.data(), cell_drawables.size());

        } else if ( render_type == RenderType::UseFrameBuffer ) {

            // Same as when using cell_buffer.
            renderer.render(fb_array[0], drawables.data(), drawables.size());

        } else if ( render_type == RenderType::UseDoubleBuffer ) {
            
            auto& current = fb_array[fb_index];
            fb_index++;
            if ( fb_index >= fb_array.size() ) {
                fb_index = 0;
            }
            // Async mode cannot clear buffer post-render,
            // so it must be pre-initialized.
            // (render() UI may need reconsideration for consistency with blocking mode)
            current.clear();
            renderer.render_async(current, drawables.data(), drawables.size());

        } else { }
    }


    return 0;
}
