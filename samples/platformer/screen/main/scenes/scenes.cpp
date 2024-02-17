/*
 * MIT License
 * (https://opensource.org/license/mit/)
 *
 * Copyright (c) 2024 nyannkov
 */
#include "scenes.hpp"
#include "scene_001.hpp"
#include "scene_002.hpp"

namespace mgc::main_screen {

std::unique_ptr<SceneIf> make_scene_by_id(SceneId id) {
    switch (id) {
    case SceneId::ID_001:
        return std::make_unique<Scene001>();
    case SceneId::ID_002:
        return std::make_unique<Scene002>();
    default:
        return nullptr;
    }
}

}
