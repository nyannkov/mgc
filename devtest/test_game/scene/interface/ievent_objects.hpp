#ifndef MGC_IEVENT_OBJECTS_HPP
#define MGC_IEVENT_OBJECTS_HPP

#include "app_common.hpp"
#include "entity/event/event.hpp"

namespace app {

struct IEventObjects {
    virtual ~IEventObjects() = default;
    virtual void init() = 0;
    virtual ArrayViewer<event::Event*> events() = 0;
    virtual void draw_effect(FramebufferT& fb, mgc::math::Vec2i& cam_pos) {
        for ( auto* evt : events() ) {
            if ( evt ) {
                evt->draw_effect(fb, cam_pos);
            }
        }
    }
};

} // namespace app

#endif/*MGC_IEVENT_OBJECTS_HPP*/

