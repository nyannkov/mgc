#ifndef MGC_WORLD_STATE_HPP
#define MGC_WORLD_STATE_HPP

#include "platform/platform.hpp"
#include "entity/player/player.hpp"
#include "checkpoint_info.hpp"
#include "equipment_info.hpp"
#include "scene_info.hpp"

namespace app {

struct WorldState {
    explicit WorldState(Platform& pf) 
        : player(
            pf.frame_timer,
            pf.gamepad,
            pf.sound_controller,
            equipment_info
          ) { }
    WorldState(const WorldState&) = delete;
    WorldState& operator=(const WorldState&) = delete;
    
    EquipmentInfo equipment_info{};
    SceneInfo scene_info{};
    CheckpointInfo checkpoint_info{};
    Player player;
};

}// namespace app

#endif/*MGC_WORLD_STATE_HPP*/

