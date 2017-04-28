#ifndef __LUATIC_FUNCTION_HPP__
#define __LUATIC_FUNCTION_HPP__

#include <experimental/tuple>
#include <functional>
#include <string>
#include <luatic/argument.hpp>
#include <luatic/common.hpp>
#include <luatic/return.hpp>
#include <luatic/returnsize.hpp>

namespace luatic {

    namespace detail {

        // ToFunction is a helper class for generating std::function
        // type from any function types.
        // Default is for Lambda expressions or Functors.
        template <class T>
        class ToFunction 
            : public ToFunction<decltype(&T::operator())> {
        };

        // Partial specialization for const member functions of classes.
        template <class C, class R, class... Args>
        class ToFunction<R(C::*)(Args...) const> {
        public:
            using Type = std::function<R(Args...)>;
        };

        // Partial specialization for non-const member functions of classes.
        template <class C, class R, class... Args>
        class ToFunction<R(C::*)(Args...)> {
        public:
            using Type = std::function<R(Args...)>;
        };

        // Partial specialization for normal function pointers.
        template <class R, class... Args>
        class ToFunction<R(*)(Args...)> {
        public:
            using Type = std::function<R(Args...)>;
        };

        // Partial specialization for std::functions.
        template <class R, class... Args>
        class ToFunction<std::function<R(Args...)> > {
        public:
            using Type = std::function<R(Args...)>;
        };


        // DefFunction_ is implementation class of DefFunction class.
        template <class T, class F>
        class DefFunction_;

        // Partial specialization for functions that returns some values.
        template <class T, class R, class... Args>
        class DefFunction_<T, std::function<R(Args...)> > {
        public:
            static auto apply(lua_State* L,
                              const std::string& functionName,
                              std::function<R(Args...)> functionBody)
                -> void {
                static auto static_func = [functionBody](lua_State* L_) -> int {
                    auto args = detail::Arguments<Args...>::apply(L_);
                    auto returns = std::experimental::apply(functionBody, args);
                    
                    luatic::detail::Returns<R>::apply(L_, returns);
                    return luatic::detail::ReturnSize<decltype(returns)>::Value;
                };

                lua_pushcfunction(L, [](lua_State* L_) -> int {
                        static_func(L_);
                    });
            }
        };

        // Partial specialization for functions that return nothing(void).
        template <class T, class... Args>
        class DefFunction_<T, std::function<void(Args...)> > {
        public:
            static auto apply(lua_State* L,
                              const std::string& functionName,
                              std::function<void(Args...)> functionBody) -> void {
                static auto static_func = [functionBody](lua_State* L_) -> int {
                    auto args = detail::Arguments<Args...>::apply(L_);
                    std::experimental::apply(functionBody, args);
                    return 0;
                };

                lua_pushcfunction(L, [](lua_State* L_) -> int {
                        static_func(L_);
                    });
            }
        };

        // Partial specialization for functions that get nothing as parameters.
        template <class T, class R>
        class DefFunction_<T, std::function<R(void)> > {
        public:
            static auto apply(lua_State* L,
                              const std::string& functionName,
                              std::function<R(void)> functionBody)
                -> void {
                static auto static_func = [functionBody](lua_State* L_) -> int {
                    auto returns = functionBody();
                    luatic::detail::Returns<R>::apply(L_, returns);
                    return luatic::detail::ReturnSize<decltype(returns)>::Value;
                };

                lua_pushcfunction(L, [](lua_State* L_) -> int {
                        static_func(L_);
                    });
            }
        };

        // Partial specialization for functions that get nothing as parameters
        // and return nothing(void).
        template <class T>
        class DefFunction_<T, std::function<void(void)> > {
        public:
            static auto apply(lua_State* L,
                              const std::string& functionName,
                              std::function<void(void)> functionBody)
                -> void {
                static auto static_func = [functionBody](lua_State* L_) -> int {
                    functionBody();
                    return 0;
                };

                lua_pushcfunction(L, [](lua_State* L_) -> int {
                        static_func(L_);
                    });
            }
        };

        // DefFunction provides only static function 'apply', which
        // register functions into lua states.
        template <class T, class F>
        class DefFunction {
        public:
            static auto apply(lua_State* L,
                              const std::string& functionName,
                              F functionBody) {
                using Function = typename luatic::detail::ToFunction<F>::Type;
                auto functionBody_ = Function(functionBody);
                luatic::detail::DefFunction_<T, Function>::apply
                    (L, functionName, functionBody_);
            }
        };

    } // namespace detail
    
} // namespace luatic

#endif // __LUATIC_FUNCTION_HPP__
