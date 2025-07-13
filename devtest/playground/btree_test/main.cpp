#include "mgc_cpp/mgc.hpp"
#include "mgc_drivers/platform/display/st7789/cpp/st7789.hpp"
#include "mgc_drivers/platform/input/digital_gamepad/cpp/digital_gamepad.hpp"
#include "mgc_drivers/platform/sound/mml_psg/cpp/sound_controller_mml_psg.hpp"
#include "mgc_drivers/platform/timer/free_running_timer/cpp/free_running_timer_u32.hpp"
#include "platform.hpp"
#include "stage/stage.hpp"
#include "player.hpp"
#include "enemy.hpp"

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

mgc_color_t buf1[240*240];
mgc_color_t buf2[240*240];
DoubleFramebuffer dfb(buf1, buf2, 240, 240);
SimpleCameraFollower camera;
mgc::render::DoubleBufferedRenderer<ST7789> renderer(dfb, display_driver, &camera);

void platform_init() {
    display_driver.init(50*1000*1000); // over clock
    sound_controller.init();
    gamepad.init(); 
}

}

devtest::Stage stage;
devtest::Player player(platform);
devtest::Enemy fish(platform);
CollisionDetectorBoxToMap collision_detector;

int main() {

    platform_init();

    stage.setup_by_id(devtest::Stage::StageId::Stage1);

    stage.set_position(mgc::math::Vec2i(0, 0));

    camera.set_x_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(28), MGC_CELL2PIXEL(3));
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

        stage.update();

        collision_detector.detect(player, stage, CornerPushDirection::PreferX);

        camera.update_follow_position();
        
        renderer.draw_to_framebuffer(drawables.data(), drawables.size());

        renderer.wait_until_idle_interrupt();

        renderer.transfer_to_display_async();
    }

    return 0;
}


