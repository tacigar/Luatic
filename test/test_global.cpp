#include <luatic/luatic.hpp>
#include <gtest/gtest.h>
#include "common.hpp"
#include "testing/testing.hpp"

struct GlobalTestMarker {};

TEST(GlobalTest, hoge) {
    auto L = luaL_newstate();
    luaL_openlibs(L);
    testing::load(L);
    
    luatic::Global<GlobalTestMarker>(L)
        .begin()
        .defFunction("f1", [](int x) -> int { return x * 2; })
        .end();

    dostring(L, "v1 = f1(2)");
    dostring(L, "assertTrue(v1 == 4)");
    
    lua_close(L);
}

