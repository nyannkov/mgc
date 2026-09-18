#ifndef MGC_WAYPOINT_CARRIER_HPP
#define MGC_WAYPOINT_CARRIER_HPP

#include <math.h>//sqrt
#include "mgc_cpp/mgc.hpp"
#include "app_common.hpp"
#include "entity/carrier/carrier.hpp"
#include "entity/player/player_hitbox_index.hpp"
#include "resources/generated/tileset/tileset_map_elements.h"
#include "resources/generated/map/map_stage_2_8_water.h"

namespace app {
namespace carrier {

using WaypointT = mgc::math::Vec2i;

enum class WaypointCarrierMode {
    OneWay,
    PingPong,
    Loop
};

enum class WaypointCarrierState {
    Stop,
    Running,
    //Wait,
    Finished
};

template <uint16_t Row, uint16_t Col>
struct WaypointCarrier : Carrier {


    explicit WaypointCarrier(const FrameTimerT& timer) : sw_(timer) {
        for ( auto& m : map_ ) {
            m = 0x81;
        }
        this->mut_tilegrid().set_tile_index_map(tile_index_map_);
        this->mut_tilegrid().set_tileset(tileset_map_elements);
        this->set_collision_map(&tile_index_map_);
    }

    ~WaypointCarrier() = default;

    void spawn(const mgc::math::Vec2i& pos) override {
        this->mut_tilegrid().set_visible(true);
        this->set_collision_enabled(true);
        this->set_off_board_counter(0);

        this->set_position(pos);
        posf_.x = pos.x;
        posf_.y = pos.y;
        velocity_ = { 0.0f, 0.0f };

        mode_ = WaypointCarrierMode::OneWay;
        state_ = WaypointCarrierState::Stop;
        dest_index_ = 0;
        wp_array_ = nullptr;
        wp_count_ = 0;
        is_reverse_ = false;
        speed_ = 1.0F;
    }

    void despawn() override {
        this->mut_tilegrid().set_visible(false);
        this->set_collision_enabled(false);
    }

    void update_movement() override {
        
        this->update_off_board_counter();

        if ( state_ != WaypointCarrierState::Running ) return;
        
        auto dest = wp_array_[dest_index_];

        if ( is_reached() ) {
            const auto& orig = wp_array_[dest_index_];
            if ( mode_ == WaypointCarrierMode::OneWay ) {
                
                if ( is_reverse_ ) {
                    if ( dest_index_ == 0 ) {
                        state_ = WaypointCarrierState::Finished;
                        velocity_ = { 0.0F, 0.0F };
                    } else {
                        dest_index_--;
                        velocity_ = calc_velocity(orig, wp_array_[dest_index_], speed_);
                    }
                } else {
                    if ( (wp_count_ - 1) <= dest_index_ ) {
                        state_ = WaypointCarrierState::Finished;
                        velocity_ = { 0.0F, 0.0F };
                    } else {
                        dest_index_++;
                        velocity_ = calc_velocity(orig, wp_array_[dest_index_], speed_);
                    }
                }

            } else if ( mode_ == WaypointCarrierMode::PingPong ) {
                if ( is_reverse_ ) {
                    if ( dest_index_ == 0 ) {
                        dest_index_++;
                        is_reverse_ = false;
                    } else {
                        dest_index_--;
                    }
                } else {
                    if ( dest_index_ == wp_count_ - 1 ) {
                        dest_index_--;
                        is_reverse_ = true;
                    } else {
                        dest_index_++;
                    }
                }

                velocity_ = calc_velocity(orig, wp_array_[dest_index_], speed_);

            } else if ( mode_ == WaypointCarrierMode::Loop ) {
                if ( is_reverse_ ) {
                    if ( dest_index_ == 0 ) {
                        dest_index_ = wp_count_ - 1;
                    } else {
                        dest_index_--;
                    }
                } else {
                    if ( dest_index_ == wp_count_ - 1 ) {
                        dest_index_ = 0;
                    } else {
                        dest_index_++;
                    }
                }

                velocity_ = calc_velocity(orig, wp_array_[dest_index_], speed_);
                
            } else {
                state_ = WaypointCarrierState::Finished;
            }
        }

        // update posf
        auto pre_posf = posf_;
        posf_ += velocity_;
        this->set_position(floor_cast(posf_));
        auto delta_next = posf_ - pre_posf; 
        this->set_delta(delta_next);
    }

    bool is_reached() const {
        
        // TODO nullptr

        auto dest = wp_array_[dest_index_];

        auto dx = static_cast<float>(dest.x) - posf_.x;
        auto dy = static_cast<float>(dest.y) - posf_.y;
        bool is_reached_x = false;
        bool is_reached_y = false;

        if ( velocity_.x > 0.0f ) {
            if ( dx <= 0.0f ) {
               is_reached_x = true; 
            }
        } else if ( velocity_.x < 0.0f ) {
            if ( dx >= 0.0f ) {
                is_reached_x = true;
            }
        } else {
            is_reached_x = true;
        }

        if ( velocity_.y > 0.0f ) {
            if ( dy <= 0.0f ) {
               is_reached_y = true; 
            }
        } else if ( velocity_.y < 0.0f ) {
            if ( dy >= 0.0f ) {
                is_reached_y = true;
            }
        } else {
            is_reached_y = true;
        }

        return is_reached_x && is_reached_y;
    }

    void resolve_movement() override {
    }

    void update_animation() override {
    }

    auto mode() const { return mode_; }
    auto state() const { return state_; }
    void set_speed(float speed) { 
        if ( speed < 0.0f ) {
            speed_ = 0.0f;
        } else {
            speed_ = speed; 
        }
    }
    auto speed() const { return speed_; }

    void start(const WaypointT* wp_array, size_t wp_count, WaypointCarrierMode mode, size_t start_index = 0) {
        
        wp_array_ = wp_array;
        wp_count_ = wp_count;
        if ( wp_array_ == nullptr || wp_count_ == 0 ) {
            state_ = WaypointCarrierState::Stop;
            return;
        }
        if ( start_index < wp_count_ ) {
            dest_index_ = start_index;
        } else {
            dest_index_ = 0;
        }
        velocity_ = calc_velocity(this->position(), wp_array_[dest_index_], speed_);
        mode_ = mode;
        state_ = WaypointCarrierState::Running;
    }

    void suspend() { }
    void go() { }
    void stop() {
        state_ = WaypointCarrierState::Stop;
        dest_index_ = 0;
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
    float speed_;
    const WaypointT* wp_array_;
    size_t wp_count_;
    WaypointCarrierState state_ = WaypointCarrierState::Stop;
    WaypointCarrierMode mode_;
    StopwatchT sw_;
    size_t dest_index_;
    bool is_reverse_;

    void on_collision_resolved(
        const carrier::Carrier& carrier,
        const mgc::collision::MapPushbackInfo& info
    ) override { 
        if ( info.pushback.y < 0 ) {
            this->set_off_board_counter(MAX_OFF_BOARD_COUNTER_);
        }
    }

    static mgc::math::Vec2f calc_velocity(mgc::math::Vec2i orig, mgc::math::Vec2i dest, float speed) {
        auto dx = static_cast<float>(dest.x - orig.x);
        auto dy = static_cast<float>(dest.y - orig.y);

        float d = sqrt(dx*dx + dy*dy);

        float vx, vy;

        if ( d != 0.0f ) {
            vx = dx / d;
            vy = dy / d;
        } else {
            vx = 0.0f;
            vy = 0.0f;
        }
        return mgc::math::Vec2f(vx, vy) * speed;
    }

    //TODO library
    static constexpr mgc_world_t floor_cast(float v) {
        mgc_world_t i = static_cast<mgc_world_t>(v);
        return (v < 0.0f && static_cast<float>(i) != v) ? (i - 1) : i;
    }
    static constexpr mgc::math::Vec2i floor_cast(const mgc::math::Vec2f& v) {
        return {
            floor_cast(v.x),
            floor_cast(v.y)
        };
    }
};

} // namespace carrier
} // namespace app

#endif // MGC_CARRIER_HPP
