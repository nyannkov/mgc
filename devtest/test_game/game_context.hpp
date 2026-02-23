#ifndef MGC_GAME_CONTEXT_HPP
#define MGC_GAME_CONTEXT_HPP

#include "platform/platform.hpp"
#include "world_state/world_state.hpp"
#include "request_hub/request_hub.hpp"

namespace app {

struct GameContext {
    GameContext(
        Platform& pf,
        WorldState& ws,
        RequestHub& req
    ) : platform(pf),
        world_state(ws),
        request_hub(req) { }
    GameContext(const GameContext&) = delete;
    GameContext& operator=(const GameContext&) = delete;

    Platform& platform;
    WorldState& world_state;
    RequestHub& request_hub;
};

} // namespace app

#endif/*MGC_GAME_CONTEXT_HPP*/

