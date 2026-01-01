#include "app_common.hpp"
#include "frame/main_frame.hpp"
#include "frame/status_frame.hpp"
#include "resources/mml/mml.h"
#include "platform/platform.hpp"

namespace {


app::Player player(app::frame_timer, app::gamepad, app::sound_controller);
app::GameContext ctx(app::display_driver, app::sound_controller, app::gamepad, app::frame_timer, player);
app::StopwatchT sw(app::frame_timer);

app::MainFrame<224, 192, app::DisplayDriverT> main_frame(8, 48, app::display_driver);
app::StatusFrame<224, 32, app::DisplayDriverT> status_frame(8, 8, app::display_driver, ctx);

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

    app::platform_init();

    app::sound_controller.set_background_music_list(bgm_records, BGM_RECORDS_COUNT);
    app::sound_controller.set_sound_effect_list(se_records, SE_RECORDS_COUNT);
    app::sound_controller.set_lpf_enabled(true);
    app::sound_controller.set_lpf_alpha(0.5);
    app::sound_controller.set_master_volume(0.5);

    sw.start();

    main_frame.set_scene(app::SceneId::Id_000, ctx);

    status_frame.clear_display_blocking();

    while (1) {

        app::platform_tick();
        app::platform_sound_proc();
        
        if ( sw.elapsed_ms() >= 25 ) {
            sw.restart();
            app::platform_gamepad_proc();
            
            if ( main_frame.update_scene() ) {
                main_frame.change_next_scene(ctx);
            }

            main_frame.draw_to_buffer();

            main_frame.sync();

            status_display_draw_blocking();

            main_frame.flush();

            if ( !app::platform_is_continue() ) {
                break;
            }

        } else {
            app::platform_sleep(1);
        }
    }

    app::platform_deinit();

    return 0;
}

