#ifndef MGC_CHECKPOINT_INFO_HPP
#define MGC_CHECKPOINT_INFO_HPP

#include "app_common.hpp"

namespace app {

enum class Checkpoint : uint32_t {
    Initial = 0,
    TowerFrontEvent_Finished,
    Corridor_OpenedTheDoor,
    Shop_FirstVisit,
    Shop_FirstTalk,
    WonTheStage1Boss,
    Puzzle1_Solved,
    WonTheStage2Boss,
};

struct CheckpointInfo {
    
    bool has_reached(Checkpoint cp) const {
        return (
            static_cast<uint32_t>(current_point_) >= static_cast<uint32_t>(cp)
        );
    }
    
    void advance_checkpoint(Checkpoint cp) {
        if ( !has_reached(cp) ) {
            current_point_ = cp;
        }
    }

    void set_checkpoint(Checkpoint cp) {
        current_point_ = cp;
    }

private:
    Checkpoint current_point_ = Checkpoint::Initial;
};

}// namespace app

#endif/*MGC_CHECKPOINT_INFO_HPP*/

