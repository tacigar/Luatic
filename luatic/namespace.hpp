/* =============================================================================
 * Copyright (c) 2017 tacigar
 * https://github.com/tacigar/Luatic
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * ============================================================================= */

#ifndef __LUATIC_NAMESPACE_HPP__
#define __LUATIC_NAMESPACE_HPP__

#include <functional>
#include <string>
#include <luatic/common.hpp>
#include <luatic/function.hpp>

namespace luatic {

    template <class Outer>
    class Namespace;
    
    namespace detail {

        template <int N>
        class Global_;
        
        template <class Outer, int N>
        class Namespace_;

        
        template <class Outer_, int M, int N>
        class Namespace_<Namespace_<Outer_, M>, N> {
            template <class O, int K>
            friend class Namespace_;
        private:
            using Outer = Namespace_<Outer_, M>;

        public:
            Namespace_(lua_State* L, const std::string& name,
                       const Outer& outer)
                : _L(L)
                , _name(name)
                , _outer(outer) {
            }

            ~Namespace_() = default;

            template <int K>
            Namespace_(const Namespace_<Outer, K>& other)
                : _L(other._L)
                , _name(other._name)
                , _outer(other._outer) {
            }

            template <int K>
            Namespace_& operator=(const Namespace_<Outer_, K>& other) {
                _L = other._L;
                _name = other._name;
                _outer = other._outer;
            }
            
            auto end() -> decltype(auto) {
                lua_setfield(_L, -2, _name.data());
                return Namespace_<Outer_, M + 1>(_outer);
            }

            template <class F>
            auto defFunction(const std::string& functionName,
                             F functionBody) -> decltype(auto) {
                luatic::detail::DefFunction<Namespace_, F>::apply
                    (_L, functionName, functionBody);
                lua_setfield(_L, -2, functionName.data());
                return Namespace_<Outer, N + 1>(*this);
            }

        private:
            lua_State* _L;
            std::string _name;
            Outer _outer;
        };

        template <int M, int N>
        class Namespace_<Global_<M>, N> {
            template <class O, int K>
            friend class luatic::detail::Namespace_;
        private:
            using Outer = Global_<M>;
            
        public:
            Namespace_(lua_State* L, const std::string& name,
                       const Outer& outer)
                : _L(L)
                , _name(name)
                , _outer(outer) {
            }

            template <int K>
            Namespace_(const Namespace_<Outer, K>& other)
                : _L(other._L)
                , _name(other._name)
                , _outer(other._outer) {
            }
            
            ~Namespace_() = default;

            auto end() -> decltype(auto) {
                lua_setglobal(_L, _name.data());
                return Global_<M + 1>(_L);
            }

            template <class F>
            auto defFunction(const std::string& functionName,
                             F functionBody) -> decltype(auto) {
                luatic::detail::DefFunction<Namespace_, F>::apply
                    (_L, functionName, functionBody);
                lua_setfield(_L, -2, functionName.data());
                return Namespace_<Outer, N + 1>(*this);
            }

            auto defNamespace(const std::string& name) {
                return luatic::Namespace<Namespace_>(_L, name, *this);
            }
            
        private:
            lua_State* _L;
            std::string _name;
            Outer _outer;
        };

    } // namespace detail

    template <class Outer>
    class Namespace {
    public:
        Namespace(lua_State* L, const std::string& name, const Outer& outer)
            : _L(L), _name(name), _outer(outer) {
        }
        
        ~Namespace() = default;
        
        auto begin() -> decltype(auto) {
            lua_newtable(_L);
            return luatic::detail::Namespace_<Outer, 0>(_L, _name, _outer);
        }

    private:
        lua_State* _L;
        std::string _name;
        Outer _outer;
    };

} // namespace luatic

#endif // __LUATIC_NAMESPACE_HPP__
