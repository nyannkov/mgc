#ifndef MGC_SIZED_CARRIER_HPP
#define MGC_SIZED_CARRIER_HPP

#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/carrier/carrier.hpp"
#include "entity/player/player_hitbox_index.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_stage_2_8_water.h"

namespace app {
namespace carrier {

template <uint16_t Row, uint16_t Col>
struct SizedCarrier : Carrier {

    SizedCarrier() {
        for ( auto& m : map_ ) {
            m = 0x81;
        }
        this->mut_tilegrid().set_tile_index_map(tile_index_map_);
        this->mut_tilegrid().set_tileset(tileset_map_elements);
        this->set_collision_map(&tile_index_map_);
    }

    ~SizedCarrier() = default;

    void spawn(const mgc::math::Vec2i& pos) override {
        this->mut_tilegrid().set_visible(true);
        this->set_collision_enabled(true);

        this->set_position(pos);
        posf_.x = pos.x;
        posf_.y = pos.y;
        velocity_.x = 0.0f;
        velocity_.y = 0.0f;
    }

    void despawn() override {
        this->mut_tilegrid().set_visible(false);
        this->set_collision_enabled(false);
    }

    void update_movement() override {
        
        //static int counter = 0;
        //counter++;
        //int16_t x = 0;
        //if ( counter < 20 ) {
        //    x = 1;
        //} else if ( counter < 40 ) {
        //    x = -1;
        //} else {
        //    counter = 0;
        //    x = 0;
        //}
        //this->set_delta({x, 0});
        //auto pos = this->position();
        //pos.x += x;
        //this->set_position(pos);
    }

    void resolve_movement() override {
    }

    void update_animation() override {
    }

private:
    uint8_t map_[Row*Col];
    TileIndexMap tile_index_map_ = {
        map_,
        countof(map_),
        Col,
        Row,
        MGC_MAP_COMPRESSION_NONE
    };
    mgc::math::Vec2f posf_;
    mgc::math::Vec2f velocity_;
};

} // namespace carrier
} // namespace app

#endif // MGC_CARRIER_HPP
