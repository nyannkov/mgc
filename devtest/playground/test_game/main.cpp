#include "app_common.hpp"
#include "frame/main_frame.hpp"
#include "frame/status_frame.hpp"

namespace {

app::DisplayDriverT display_driver;
app::SoundControllerT sound_controller;
app::GamepadT& gamepad = mgc::drivers::platform::input::default_gamepad();
app::FrameTimerT frame_timer;

app::GameContext ctx(display_driver, sound_controller, gamepad, frame_timer);
app::MainFrame<224, 192, app::DisplayDriverT> main_frame(8, 48, display_driver);
app::StatusFrame<224, 32, app::DisplayDriverT> status_frame(8, 8, display_driver, ctx);

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

void status_display_draw_blocking() {

    static bool show_status_flag = false;
    enum class StatusDisplayControl {
        None,
        Show,
        Hide,
        Update
    } control = StatusDisplayControl::None;

    status_frame.update();

    auto request_type = ctx.status_display_request().request();
    if ( show_status_flag ) {
        if ( request_type == app::StatusDisplayRequestType::Hide ) {
            control = StatusDisplayControl::Hide;
            show_status_flag = false;
        } else {
            if ( status_frame.has_update() ) {
                status_frame.clear_update();
                control = StatusDisplayControl::Update;
            } else {
                control = StatusDisplayControl::None;
            }
        }
    } else {
        if ( request_type == app::StatusDisplayRequestType::Show ) {
            control = StatusDisplayControl::Show;
            show_status_flag = true;
        } else {
            control = StatusDisplayControl::None;
        }
    }
    ctx.status_display_request().clear_request();

    switch (control) {
    case StatusDisplayControl::Show:/*fall-through*/
    case StatusDisplayControl::Update:
        status_frame.draw_blocking();
        break;

    case StatusDisplayControl::Hide:
        status_frame.clear_display_blocking();
        break;

    default:
        break;
    }
}

}

int main() {

    platform_init();

    main_frame.set_scene(app::SceneId::Id_000, ctx);

    status_frame.clear_display_blocking();

    while (1) {
        
        platform_proc();
        
        if ( main_frame.update_scene() ) {
            main_frame.change_next_scene(ctx);
        }

        main_frame.draw_to_buffer();

        main_frame.sync();

        status_display_draw_blocking();

        main_frame.flush();
    }

    return 0;
}

