#include <cstdlib>
#include <iostream>
#include <tuple>
#include <luatic/global.hpp>

auto odds() -> decltype(auto) {
    return std::make_tuple(1, 3, 5, 7, 9);
}

auto main() -> int {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "main.lua")) {
        std::cerr << "Cannot open main.lua." << std::endl;
        return EXIT_FAILURE;
    }

    luatic::Global<>(L)
        .begin()
            .defFunction("odds", odds)
        .end();
    
    if (lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Cannot exec main.lua:" << lua_tostring(L, -1) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
