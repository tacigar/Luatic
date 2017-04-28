#include <cmath>
#include <cstdlib>
#include <iostream>
#include <luatic/namespace.hpp>
#include <luatic/global.hpp>

auto main() -> int {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_loadfile(L, "main.lua")) {
        std::cerr << "Cannot open main.lua." << std::endl;
        return EXIT_FAILURE;
    }

    luatic::Global<>(L)
        .begin()
            .defNamespace("std")
            .begin()
                .defNamespace("math")
                .begin()
                    .defFunction("sin", [](double x)->double { return std::sin(x); })
                    .defFunction("cos", [](double x)->double { return std::cos(x); })
                    .defFunction("tan", [](double x)->double { return std::tan(x); })
                .end()
                .defNamespace("io")
                .begin()
                    .defFunction("print_number", [](double x)->void {
                            std::cout << x << std::endl;
                        })
                    .defFunction("print_string", [](const std::string& s)->void {
                            std::cout << s << std::endl;
                        })
                .end()
            .end()
        .end();
    
    if (lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Cannot exec main.lua:" << lua_tostring(L, -1) << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
