#include <catch2/catch_test_macros.hpp>
#include "mgc_cpp/graphics/cell_buffer.hpp"
#include <type_traits>

using namespace mgc::graphics;

TEST_CASE("CellBuffer type traits") {
    using mgc::graphics::CellBuffer;

    static_assert(!std::is_copy_constructible_v<CellBuffer>, "Should not be copy constructible");
    static_assert(!std::is_copy_assignable_v<CellBuffer>, "Should not be copy assignable");
    static_assert(std::is_move_constructible_v<CellBuffer>, "Should be move constructible");
    static_assert(std::is_move_assignable_v<CellBuffer>, "Should be move assignable");
}

TEST_CASE("CellBuffer basic usage") {
    mgc::graphics::CellBuffer cb;
    cb.clear();

    REQUIRE(cb.width() > 0);
    REQUIRE(cb.height() > 0);
    REQUIRE(cb.data() != nullptr);
    REQUIRE(cb.data_bytes() != nullptr);
}

