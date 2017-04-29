/* =============================================================================
 * Copyright (c) 2017 tacigar
 * https://github.com/tacigar/Luatic
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * ============================================================================= */

#ifndef __LUATIC_ARGUMENT_HPP__
#define __LUATIC_ARGUMENT_HPP__

#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>
#include <luatic/common.hpp>

namespace luatic {

    namespace detail {

        template <int N, class Arg>
        class Argument_;

        template <int N>
        class Argument_<N, bool> {
        public:
            static auto apply(lua_State* L) -> bool {
                return lua_toboolean(L, N);
            }
        };

        template <int N>
        class Argument_<N, int> {
        public:
            static auto apply(lua_State* L) -> int {
                return luaL_checkint(L, N);
            }
        };

        template <int N>
        class Argument_<N, float> {
        public:
            static auto apply(lua_State* L) -> float {
                return luaL_checknumber(L, N);
            }
        };

        template <int N>
        class Argument_<N, double> {
        public:
            static auto apply(lua_State* L) -> double {
                return luaL_checknumber(L, N);
            }
        };
        
        template <int N>
        class Argument_<N, std::string> {
        public:
            static auto apply(lua_State* L) -> std::string {
                std::size_t length;
                auto s = luaL_checklstring(L, N, &length);
                return std::string(s, s + length);
            }
        };
        
        
        template <int N, class Arg>
        class Argument {
        public:
            static auto apply(lua_State* L) -> decltype(auto) {
                return Argument_<N, typename std::decay<Arg>::type>::apply(L);
            }
        };

        
        template <int N, class... Args>
        class Arguments_;

        template <int N, class Arg1, class... Args>
        class Arguments_<N, Arg1, Args...> {
        public:
            static auto apply(lua_State* L) -> decltype(auto) {
                auto arg1 = Argument<N, Arg1>::apply(L);
                auto args = Arguments_<N + 1, Args...>::apply(L);
                
                return std::tuple_cat(std::make_tuple(arg1), args);
            }
        };

        template <int N, class Arg1>
        class Arguments_<N, Arg1> {
        public:
            static auto apply(lua_State* L) -> decltype(auto) {
                auto arg1 = Argument<N, Arg1>::apply(L);
                return std::make_tuple(arg1);
            }
        };
        
        template <int N>
        class Arguments_<N> {
        public:
            static auto apply(lua_State* L) -> decltype(auto) {
                std::make_tuple();
            }
        };

        
        template <class... Args>
        class Arguments {
        public:
            static auto apply(lua_State* L) -> decltype(auto) {
                return Arguments_<1, Args...>::apply(L);
            }
        };
        
    } // namespace detail
    
} // namespace luatic

#endif // __LUATIC_ARGUMENT_HPP__
