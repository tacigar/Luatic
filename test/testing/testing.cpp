#include <gtest/gtest.h>
#include "testing.hpp"

namespace {

    auto assertTrue(bool condition) -> void {
        ASSERT_TRUE(condition);
    }

    auto assertFalse(bool condition) -> void {
        ASSERT_FALSE(condition);
    }

    auto expectTrue(bool condition) -> void {
        EXPECT_TRUE(condition);
    }

    auto expectFalse(bool condition) -> void {
        EXPECT_FALSE(condition);
    }

} // namespace

namespace testing {

    struct TestingMarker {};
    
    auto load(lua_State* L) -> void {
        luatic::Global<TestingMarker>(L)
            .begin()
            .defFunction("assertTrue", assertTrue)
            .defFunction("assertFalse", assertFalse)
            .defFunction("expectTrue", expectTrue)
            .defFunction("expectFalse", expectFalse)
            .end();
    }
    
} // namespace testing
