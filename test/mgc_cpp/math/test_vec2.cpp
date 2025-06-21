#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "mgc_cpp/math/vec2.hpp"

TEMPLATE_TEST_CASE("Vec2 operations work for multiple types", "[Vec2][template]", int16_t, uint16_t) {
    using Vec = mgc::math::Vec2<TestType>;

    SECTION("default constructor") {
        Vec v;
        REQUIRE(v.x == TestType(0));
        REQUIRE(v.y == TestType(0));
    }

    SECTION("value constructor") {
        Vec v(TestType(1), TestType(-2));
        REQUIRE(v.x == TestType(1));
        REQUIRE(v.y == TestType(-2));
    }

    SECTION("operator+") {
        Vec a(TestType(3), TestType(4));
        Vec b(TestType(1), TestType(2));
        Vec c = a + b;
        REQUIRE(c.x == TestType(4));
        REQUIRE(c.y == TestType(6));
    }

    SECTION("operator+=") {
        Vec a(TestType(3), TestType(4));
        Vec b(TestType(1), TestType(2));
        a += b;
        REQUIRE(a.x == TestType(4));
        REQUIRE(a.y == TestType(6));
    }
}

