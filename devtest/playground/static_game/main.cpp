#include "mgc_cpp/mgc.hpp"
#include "app_typedefs.hpp"
#include "game_scene/game_scene_manager.hpp"

namespace {

app::InputT&   gamepad = mgc::drivers::platform::input::default_gamepad();
app::DisplayT  display_driver;
app::SoundT    sound_controller;
app::TimerT    timer_u32;

app::PlatformT platform(
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

app::GameSceneManager game_scene_manager(platform);

}

int main() {

    platform_init();

    game_scene_manager.set_scene(app::GameSceneId::Scene1);

    while (1) {
        game_scene_manager.run();
    }

    return 0;
}

