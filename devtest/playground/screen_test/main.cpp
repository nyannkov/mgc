#include "mgc_cpp/mgc.hpp"
#include "game_context.hpp"
#include "platform.hpp"
#include "stage/stage.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "screen/fixed_double_buffered_screen.hpp"
#include "screen/status_screen.hpp"
#include "app_typedefs.hpp"

using mgc::collision::CollisionDetectorBoxToMap;
using mgc::collision::CollisionDetectorBoxToBox;
using mgc::collision::Hitbox;
using mgc::collision::HitboxOffset;
using mgc::collision::HitboxSize;
using CornerPushDirection = mgc::collision::CollisionDetectorBoxToMap::CornerPushDirection;

namespace {

app::InputT&   gamepad = mgc::drivers::platform::input::default_gamepad();
app::DisplayT  display_driver;
app::SoundT    sound_controller;
app::TimerT    timer_u32;

app::Platform platform(
    gamepad,
    sound_controller, 
    display_driver,
    timer_u32
);

void platform_init() {
    display_driver.init(50*1000*1000); // over clock
    sound_controller.init();
    gamepad.init(); 
}

app::Stage stage;
app::Player player;
app::Enemy enemy;

app::GameContext game_context(
    platform.input(),
    platform.sound(),
    player
);

using StatusScreenT = app::StatusScreen<
    8, 8, 224, 32, app::DisplayT, app::GameContext
>;

using MainScreenT = app::FixedDoubleBufferedScreen<
    8, 48, 224, 192, app::DisplayT, app::GameContext
>;

StatusScreenT status_screen(platform.display_impl());
MainScreenT main_screen(platform.display_impl());

CollisionDetectorBoxToMap collision_detector;

}

int main() {

    platform_init();

    stage.setup_by_id(app::Stage::StageId::Stage1);
    stage.set_position(mgc::math::Vec2i(0, 0));

    auto& camera = main_screen.camera();
    camera.set_target(player);
    camera.set_x_follow_setting(MGC_CELL2PIXEL(6), MGC_CELL2PIXEL(27), MGC_CELL2PIXEL(3));
    camera.set_y_follow_setting(MGC_CELL2PIXEL(3), MGC_CELL2PIXEL(31), MGC_CELL2PIXEL(3));
    camera.set_x_follow_enabled(true);
    camera.set_y_follow_enabled(true);

    main_screen.register_drawable(stage.bg_layer());
    main_screen.register_drawable(stage.main_layer());
    main_screen.register_drawable(&enemy);
    main_screen.register_drawable(&player);

    main_screen.register_updatable(&enemy);
    main_screen.register_updatable(&player);

    while (1) {

        platform.input_impl().proc();

        stage.update();

        main_screen.update(game_context);

        status_screen.update(game_context);

        collision_detector.detect(player, stage, CornerPushDirection::PreferX);
        CollisionDetectorBoxToBox::detect(player, enemy);

        main_screen.draw_to_buffer();

        main_screen.sync();

        status_screen.draw_blocking_if_needed();

        main_screen.flush();

    }

    return 0;
}


