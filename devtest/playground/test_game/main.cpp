#include "app_common.hpp"
#include "frame/main_frame.hpp"

namespace {

app::DisplayDriverT display_driver;
app::SoundControllerT sound_controller;
app::GamepadT& gamepad = mgc::drivers::platform::input::default_gamepad();
app::FrameTimerT frame_timer;

app::GameContext ctx(display_driver, sound_controller, gamepad, frame_timer);
app::MainFrame<224, 192, app::DisplayDriverT> main_frame(8, 48, display_driver);

void platform_init() {
    display_driver.init(50*1000*1000); // over-clock
    sound_controller.init();
    gamepad.init();
    frame_timer.reset();
}

void platform_proc() {
    gamepad.proc();
    frame_timer.tick();
}

}

int main() {

    platform_init();

    main_frame.set_scene(app::SceneId::Id_000, ctx);

    while (1) {
        
        platform_proc();
        
        if ( main_frame.update_scene(ctx) ) {
            main_frame.change_next_scene(ctx);
        }

        main_frame.draw_to_buffer();
        main_frame.sync();

        main_frame.flush();
    }

    return 0;
}

