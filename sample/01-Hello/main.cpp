#include <cstdlib>
#include <iostream>
#include <luatic/global.hpp>

auto hello_from_the_other_side(const std::string& name) -> void {
    std::cout << "Hello " << name << " from the other side." << std::endl;
}

auto main() -> int {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "main.lua")) {
        std::cerr << "Cannot open main.lua." << std::endl;
        return EXIT_FAILURE;
    }

    luatic::Global(L)
        .begin()
            .defFunction("hello_from_the_other_side", hello_from_the_other_side)
            // use positive lambda, not normal lambda.
            .defFunction("hello_from_the_outside", 
                         [](const std::string& name) -> void {
                             std::cout << "Hello " << name 
                                       << " from the outside." << std::endl;
                })
        .end();
    
    if (lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Cannot exec main.lua:" << lua_tostring(L, -1) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
