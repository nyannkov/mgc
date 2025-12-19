#ifndef MGC_ENEMY_POOL_HPP
#define MGC_ENEMY_POOL_HPP

#include <cstddef>
#include <cassert>
#include <type_traits>
#include <utility>
#include "enemy.hpp"

namespace app {
namespace enemy {

template <size_t MaxObjects = 32, size_t BufferSize = 1024*8>
struct EnemyPool {
    EnemyPool() = default;

    ~EnemyPool() { reset_all(); }

    /// Create a new Enemy-derived object in the pool
    template <typename T, typename... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of_v<Enemy, T>, "T must inherit from Enemy");
        assert(count < MaxObjects && "EnemyPool object count exceeded");

        // Calculate alignment padding
        size_t current = reinterpret_cast<uintptr_t>(storage_ + offset);
        size_t padding = (alignof(T) - (current % alignof(T))) % alignof(T);
        size_t total_size = offset + padding + sizeof(T);

        if ( total_size > BufferSize ) {
            return nullptr;
        }

        void* ptr = storage_ + offset + padding;
        T* enemy = new (ptr) T(std::forward<Args>(args)...);

        allocated_[count++] = enemy;

        offset += padding + sizeof(T);
        return enemy;
    }

    void reset_all() {
        for ( size_t i = 0; i < count; i++ ) {
            allocated_[i]->~Enemy();
        }
        count = 0;
        offset = 0;
    }

private:
    alignas(std::max_align_t) uint8_t storage_[BufferSize];
    size_t offset = 0;

    std::array<Enemy*, MaxObjects> allocated_{}; // fixed-size pointer array
    size_t count = 0;
};

} // namespace enemy
} // namespace app

#endif // MGC_ENEMY_POOL_HPP

