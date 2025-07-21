#ifndef MGC_BT_LISTENER_ENEMY_HPP
#define MGC_BT_LISTENER_ENEMY_HPP

#include "app_typedefs.hpp"


namespace app {

struct Enemy;

struct BTListenerEnemy : IBTreeListenerT {
    using LeafResult = typename IBTreeListenerT::LeafResult;
    using DurationT  = typename IBTreeListenerT::DurationT;

    explicit BTListenerEnemy(Enemy& enemy) 
        : enemy_(enemy) {}


    LeafResult on_proc_leaf(
        std::string_view id,
        const DurationT& duration,
        mgc_btree_tag_t tag
    ) override  {
        return on_proc_leaf_dispatch(enemy_, id, duration, tag);
    }

private:
    Enemy& enemy_;

    static LeafResult on_proc_leaf_dispatch(
        Enemy& enemy,
        std::string_view id,
        const DurationT& duration,
        mgc_btree_tag_t tag
    );

};

}// namespace app

#endif/*MGC_BT_LISTENER_ENEMY_HPP*/
