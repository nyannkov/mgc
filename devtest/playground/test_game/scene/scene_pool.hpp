#ifndef MGC_SCENE_POOL_HPP
#define MGC_SCENE_POOL_HPP

#include <cstddef>
#include <new>
#include "mgc_cpp/mgc.hpp"
#include "scene_base.hpp"

namespace app {

struct ScenePool {
    ScenePool() : scene_base_(nullptr) {}
    ~ScenePool() = default;
    ScenePool(const ScenePool&) = delete;
    ScenePool& operator=(const ScenePool&) = delete;
    ScenePool(ScenePool&&) = default;
    ScenePool& operator=(ScenePool&&) = default;

    template <typename T, typename... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of_v<SceneBase, T>, "T must inherit from SceneBase");

        destroy();
        assert(sizeof(T) <= sizeof(storage_));

        scene_base_ = new (storage_) T(std::forward<Args>(args)...);
        return static_cast<T*>(scene_base_);
    }

    void destroy() {
        if ( scene_base_ ) {
            scene_base_->~SceneBase();
            scene_base_ = nullptr;
        }
    }

    SceneBase* get() { return scene_base_; }

private:
    alignas(std::max_align_t) uint8_t storage_[1024*4];
    SceneBase* scene_base_;
};

} // namespace app

#endif/*MGC_SCENE_POOL_HPP*/

