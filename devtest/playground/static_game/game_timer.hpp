#ifndef MGC_GAME_TIMER_HPP
#define MGC_GAME_TIMER_HPP

#include "app_typedefs.hpp"

namespace app {

inline FrameTimerT& game_timer() {
    static FrameTimerT instance;
    return instance;
}

}

#endif/*MGC_GAME_TIMER_HPP*/

