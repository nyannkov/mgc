#ifndef MGC_STATUS_HPP
#define MGC_STATUS_HPP

#include "mgc_cpp/mgc.hpp"
#include "game_context/game_context.hpp"

namespace app {

struct Status {
    
    static constexpr size_t MAX_CELL_DRAWABLES = 16;
    
    explicit Status(GameContext& ctx);
    ~Status() = default;
    void update();
    auto cell_drawables() const { return cell_drawables_.data(); }
    size_t cell_drawables_count() const { return count_cell_drawables_; }
    bool is_updated() const { return updated_; }
    void clear_updated() { updated_ = false; }
   
private:
    const Player& player_;
    int32_t disp_player_hp_;
    std::array<mgc::parts::BasicSprite, Player::MAX_PLAYER_HP> hearts_;
    std::array<const mgc::features::CellDrawable*, MAX_CELL_DRAWABLES> cell_drawables_;
    size_t count_cell_drawables_;
    bool updated_;

    void set_hearts(int32_t player_hp);
    bool add_cell_drawable(mgc::features::CellDrawable& obj);
    void clear_cell_drawables();
};

}// namespace app

#endif/*MGC_STATUS_HPP*/

