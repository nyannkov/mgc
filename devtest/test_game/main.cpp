#include "app_common.hpp"
#include "frame/main_frame.hpp"
#include "frame/status_frame.hpp"
#include "platform/platform.hpp"
#include "resources/mml/mml.h"

namespace {

auto& platform = app::get_platform_ref();

app::RequestHub request_hub;
app::WorldState world_state(platform);
app::GameContext ctx(platform, world_state, request_hub);
app::StopwatchT sw(platform.frame_timer);

app::MainFrame<app::MAIN_FRAME_WIDTH, app::MAIN_FRAME_HEIGHT> main_frame(8, 48, ctx);
app::StatusFrame<224, 32> status_frame(8, 8, ctx);

void status_display_draw_blocking() {

    static bool show_status_flag = false;
    enum class StatusDisplayControl {
        None,
        Show,
        Hide,
        Update
    } control = StatusDisplayControl::None;

    status_frame.update();

    auto& status_display_request = request_hub.status_display_request;

    auto request_type = status_display_request.request();
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
    status_display_request.clear_request();

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

    platform.sound_controller.set_background_music_list(bgm_records, BGM_RECORDS_COUNT);
    platform.sound_controller.set_sound_effect_list(se_records, SE_RECORDS_COUNT);

    sw.start();

    main_frame.init();

    status_frame.clear_display_blocking();

    while (1) {

        app::platform_tick();
        app::platform_sound_proc();
        
        auto elapsed = sw.elapsed_ms();
        if ( sw.elapsed_ms() >= 25 ) {

            sw.restart();
            app::platform_gamepad_proc();
            
            main_frame.update_screen();

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

