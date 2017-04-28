/* =============================================================================
 * Copyright (c) 2017 tacigar
 * https://github.com/tacigar/Luatic
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * ============================================================================= */

#ifndef __LUATIC_RETURN_HPP__
#define __LUATIC_RETURN_HPP__

#include <tuple>

namespace luatic {

    namespace detail {

        template <class T>
        class Return;

        template <>
        class Return<int> {
        public:
            static auto apply(lua_State* L, int value) -> void {
                lua_pushinteger(L, value);
            }
        };

        template <>
        class Return<float> {
        public:
            static auto apply(lua_State* L, float value) -> void {
                lua_pushnumber(L, value);
            }
        };

        template <>
        class Return<double> {
        public:
            static auto apply(lua_State* L, float value) -> void {
                lua_pushnumber(L, value);
            }
        };

        template <class T>
        class Returns {
        public:
            static auto apply(lua_State* L, T value) -> void {
                Return<T>::apply(L, value);
            }
        };

        template <class... Ts>
        class Returns<std::tuple<Ts...> > {
        public:
            static auto apply(lua_State* L, const std::tuple<Ts...>& returns)
                -> void {
                
            }
        };

    } // namespace detail
    
} // namespace luatic

#endif // __LUATIC_RETURN_HPP__
