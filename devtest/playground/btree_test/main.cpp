#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"
#include "platform.hpp"
#include "stage/stage.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "resources/generated/tileset/tileset_items.h"

namespace {

using mgc::drivers::platform::display::ST7789;
using mgc::drivers::platform::sound::MmlPsgSoundController;
using mgc::drivers::platform::timer::FreeRunningTimerU32;
using mgc::collision::CollisionDetectorBoxToMap;
using mgc::collision::CollisionDetectorBoxToBox;
using mgc::collision::Hitbox;
using mgc::collision::HitboxOffset;
using mgc::collision::HitboxSize;
using CornerPushDirection = mgc::collision::CollisionDetectorBoxToMap::CornerPushDirection;
using mgc::graphics::DoubleFramebuffer;
using mgc::graphics::Framebuffer;
using mgc::graphics::CellBuffer;
using mgc::platform::input::Key;
using mgc::camera::SimpleCameraFollower;

ST7789 display_driver;
MmlPsgSoundController sound_controller;
auto& gamepad = mgc::drivers::platform::input::default_gamepad();
FreeRunningTimerU32 timer_u32;

devtest::Platform platform(
    gamepad,
    sound_controller, 
    display_driver,
    timer_u32
);

mgc_color_t buf1[224*192];
mgc_color_t buf2[224*192];
DoubleFramebuffer dfb(buf1, buf2, 224, 192);
CellBuffer cell_buffer;
Framebuffer fb(buf1, 224, 192);
SimpleCameraFollower camera;
mgc::render::DoubleBufferedRenderer<ST7789> renderer(dfb, display_driver, &camera);
mgc::render::Renderer<ST7789> single_renderer(fb, display_driver, &camera);
mgc::render::CellRenderer<ST7789> cell_renderer(cell_buffer, display_driver, nullptr);

void platform_init() {
    display_driver.init(50*1000*1000); // over clock
    sound_controller.init();
    gamepad.init(); 
}

void display_clear() {

    //uint16_t display_width = display_driver.width();
    //uint16_t display_height = display_driver.height();

    cell_renderer.cell_buffer().set_back_color(MGC_COLOR(0.2f, 0.0f, 0.1f));
    cell_renderer.cell_buffer().clear();

    for ( uint16_t col = 0; col < 15; col++ ) {
        for ( uint16_t row = 0; row < 15; row++ ) {
            cell_renderer.transfer_to_display_cell_blocking(row, col);
        }
    }
}

template <typename CellRenderT>
struct StatusRegion {

    explicit StatusRegion(CellRenderT& cell_render) 
        : cell_renderer_(cell_renderer),
          is_update_life_(true) {

        heart1_.set_tileset(tileset_items);
        heart1_.set_tile_index(2);
        heart1_.set_visible(true);
        heart1_.set_position(mgc::math::Vec2i(0, 0));
        heart1_.set_parallax_factor(mgc::graphics::ParallaxFactor(0.0f, 0.0f));

        heart2_.set_tileset(tileset_items);
        heart2_.set_tile_index(2);
        heart2_.set_visible(true);
        heart2_.set_position(mgc::math::Vec2i(8, 0));
        heart1_.set_parallax_factor(mgc::graphics::ParallaxFactor(0.0f, 0.0f));

        twin_hearts_[0] = static_cast<mgc::features::CellDrawable*>(&heart1_);
        twin_hearts_[1] = static_cast<mgc::features::CellDrawable*>(&heart2_);

        cell_renderer_.cell_buffer().set_back_color(MGC_COLOR_BLACK);
        cell_renderer_.cell_buffer().clear();

        life_value_ = 10;
    }

    void inclement_life() {
        if ( life_value_ < 48 ) {
            life_value_++;
            is_update_life_ = true;
        }
    }

    void declement_life() {
        if ( 0 < life_value_ ) {
            life_value_--;
            is_update_life_ = true;
        }
    }

    void draw_blocking() {

        if ( !is_update_life_ ) {
            return;
        }

        while ( cell_renderer.is_busy() );

        is_update_life_ = false;

        auto draw_life = life_value_;

        for ( uint16_t row = 0; row < 2; row++ ) {
            for ( uint16_t col = 1; col < 13; col++ ) {

                if ( 2 <= draw_life ) {
                    heart1_.set_tile_index(1);
                    heart2_.set_tile_index(1);
                    draw_life -= 2;
                } else if ( 1 == draw_life ) {
                    heart1_.set_tile_index(1);
                    heart2_.set_tile_index(2);
                    draw_life -= 1;
                } else {
                    heart1_.set_tile_index(2);
                    heart2_.set_tile_index(2);
                }
                
                cell_renderer.draw_to_cell_buffer(
                    0, 0, twin_hearts_.data(), twin_hearts_.size()
                );

                cell_renderer.transfer_to_display_cell_blocking(row, col, 8, 8);

            }
        }
    }
    
private:
    CellRenderT& cell_renderer_;
    uint32_t life_value_;
    bool is_update_life_;
    mgc::parts::BasicSprite heart1_;
    mgc::parts::BasicSprite heart2_;
    std::array<const mgc::features::CellDrawable*, 2> twin_hearts_;
};


StatusRegion status_region(cell_renderer);
devtest::Stage stage;
devtest::Player player(platform);
devtest::Enemy fish(platform);
CollisionDetectorBoxToMap collision_detector;

size_t sleep_counter;
size_t hit_counter;

}

int main() {

    platform_init();

    display_clear();

    status_region.draw_blocking();

    stage.setup_by_id(devtest::Stage::StageId::Stage1);

    stage.set_position(mgc::math::Vec2i(0, 0));

    camera.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
    camera.set_x_follow_enabled(true);
    camera.set_y_follow_enabled(true);
    camera.set_target(player);
    camera.set_target(player);

    std::array<const mgc::features::Drawable*, 4> 
    drawables {
        stage.bg_layer(),
        stage.main_layer(),
        &fish,
        &player
    };

    while (1) {

        gamepad.proc();

        fish.update(player.position().template cast_to<float>());

        player.update();
        if ( player.is_sleep() ) {
            if ( ++sleep_counter >= 100 ) {
                sleep_counter = 0;
                status_region.inclement_life();
            }
        } else {
            sleep_counter = 0;
        }

        stage.update();

        collision_detector.detect(player, stage, CornerPushDirection::PreferX);
        CollisionDetectorBoxToBox::detect(player, fish);

        const size_t MAX_HIT_COUNTER = 30;

        if ( player.is_enemy_hit() ) {
            if ( hit_counter == MAX_HIT_COUNTER ) {
                status_region.declement_life();
                hit_counter--;
            } else {
                if ( hit_counter > 0 ) {
                    hit_counter--;
                } else {
                    hit_counter = MAX_HIT_COUNTER;
                }
            }
        } else {
            hit_counter = MAX_HIT_COUNTER;
        }

        camera.update_follow_position();

        //single_renderer.wait_until_idle_interrupt();
        
        renderer.draw_to_framebuffer(drawables.data(), drawables.size());
        //single_renderer.draw_to_framebuffer(drawables.data(), drawables.size());

        renderer.wait_until_idle_interrupt();
        status_region.draw_blocking();

        renderer.transfer_to_display_async_at(8, 16*2+8);
        //single_renderer.transfer_to_display_async_at(8, 16*2+8);
        //single_renderer.transfer_to_display_blocking_at(8, 16*2+8);
    }

    return 0;
}


