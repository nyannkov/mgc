/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "scenes.hpp"
#include "scene_001.hpp"

namespace mgc::screen {

std::unique_ptr<SceneIf> make_scene_by_id(SceneId id) {
    switch (id) {
    case SceneId::ID_001:
        return std::make_unique<Scene001>();
    default:
        return nullptr;
    }
}

}
