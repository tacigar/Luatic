/* =============================================================================
 * Copyright (c) 2017 tacigar
 * https://github.com/tacigar/Luatic
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * ============================================================================= */

#ifndef __LUATIC_GLOBAL_HPP__
#define __LUATIC_GLOBAL_HPP__

#include <functional>
#include <string>
#include <luatic/common.hpp>
#include <luatic/function.hpp>
#include <luatic/empty.hpp>

namespace luatic {

    template <class Outer>
    class Namespace;
    
    namespace detail {
        
        template <class Marker, int N>
        class Global_ {
        public:
            Global_(lua_State* L) : _L(L) {}
            ~Global_() = default;

            auto end() -> decltype(auto) {
                return Global_<Marker, N>(_L);
            }

            template <class F>
            auto defFunction(const std::string& functionName,
                             F functionBody) -> decltype(auto) {
                luatic::detail::DefFunction<Global_<Marker, N>, F>::apply
                    (_L, functionName, functionBody);
                lua_setglobal(_L, functionName.data());
                return luatic::detail::Global_<Marker, N + 1>(_L);
            }

            auto defNamespace(const std::string& name) -> decltype(auto) {
                return luatic::Namespace<Global_>(_L, name, *this);
            }
            
        private:
            lua_State* _L;
        };

    } // namespace detail

    template <class Marker = luatic::Empty>
    class Global {
    public:
        Global(lua_State* L) : _L(L) {}
        ~Global() = default;

        // All constructors generated automatically and assignment
        // operator overloadings are not permitted.
        Global(const Global&) = delete;
        Global(Global&&) = delete;
        Global() = delete;
        Global& operator=(const Global&) = delete;
        Global& operator=(Global&&) = delete;

        // You must call begin method for registering all elements of Lua.
        auto begin() -> decltype(auto) {
            return detail::Global_<Marker, 0>(_L);
        }
        
    private:
        lua_State* _L;
    };
    
} // namespace luatic

#endif // __LUATIC_GLOBAL_HPP__
