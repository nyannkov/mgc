#ifndef MGC_ISCENE_OBJECTS_HPP
#define MGC_ISCENE_OBJECTS_HPP

#include "app_common.hpp"
#include "entity/prop/prop.hpp"
#include "entity/civilian/civilian.hpp"
#include "entity/enemy/enemy.hpp"
#include "entity/item/item.hpp"
#include "entity/block/block.hpp"

namespace app {

struct ISceneObjects {
    virtual ~ISceneObjects() = default;
    virtual void init() = 0;
    virtual ArrayViewer<prop::Prop*> props() = 0;
    virtual ArrayViewer<civilian::Civilian*> civils() = 0;
    virtual ArrayViewer<enemy::Enemy*> enemies() = 0;
    virtual ArrayViewer<item::Item*> items() = 0;
    virtual ArrayViewer<block::Block*> blocks() = 0;

    virtual void draw(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) {
        for ( auto* civil : civils() ) {
            civil->draw(fb, cam_pos);
        }
        for ( auto* prop : props() ) {
            prop->draw(fb, cam_pos);
        }
        for ( auto* enemy : enemies() ) {
            enemy->draw(fb, cam_pos);
        }
        for ( auto* item : items() ) {
            item->draw(fb, cam_pos);
        }
        for ( auto* block : blocks() ) {
            block->draw(fb, cam_pos);
        }
    }

    virtual void draw_after(
        FramebufferT& fb,
        mgc::math::Vec2i& cam_pos
    ) { }
};

} // namespace app

#endif/*MGC_ISCENE_OBJECTS_HPP*/

