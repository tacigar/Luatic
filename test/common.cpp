#include <iostream>
#include "common.hpp"

auto dostring(lua_State* L, const std::string& str) -> void {
    auto error = luaL_loadstring(L, str.data()) ||
        lua_pcall(L, 0, 0, 0);

    if (error) {
        std::cerr << "Cannot execute: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}
