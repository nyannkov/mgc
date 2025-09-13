#ifndef MGC_PLAYER_STATS_HPP
#define MGC_PLAYER_STATS_HPP

#include "mgc_cpp/mgc.hpp"

namespace app {

struct PlayerStats {
    void add_gold(int32_t amount) { gold_ += amount; }
    int32_t gold() const { return gold_; }

private:
    int32_t gold_ = 0;
};

}// namespace app

#endif/*MGC_PLAYER_STATS_HPP*/

