#include <catch2/catch_test_macros.hpp>
#include "mgc_cpp/graphics/framebuffer.hpp"
#include <type_traits>

using namespace mgc::graphics;

TEST_CASE("Framebuffer type traits") {
    static_assert(!std::is_copy_constructible_v<Framebuffer>, "Framebuffer should not be copy constructible");
    static_assert(!std::is_copy_assignable_v<Framebuffer>, "Framebuffer should not be copy assignable");

    static_assert(std::is_move_constructible_v<Framebuffer>, "Framebuffer should be move constructible");
    static_assert(std::is_move_assignable_v<Framebuffer>, "Framebuffer should be move assignable");
}

TEST_CASE("Framebuffer basic construction") {
    constexpr uint16_t WIDTH = 4;
    constexpr uint16_t HEIGHT = 4;
    mgc::graphics::Color buffer[WIDTH * HEIGHT];

    Framebuffer fb(buffer, WIDTH, HEIGHT);

    REQUIRE(fb.width() == WIDTH);
    REQUIRE(fb.height() == HEIGHT);
    REQUIRE(fb.data() == buffer);
    REQUIRE(fb.size() == sizeof(mgc::graphics::Color) * WIDTH * HEIGHT);
}

