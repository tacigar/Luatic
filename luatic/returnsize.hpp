#ifndef __LUATIC_RETURN_SIZE_HPP__
#define __LUATIC_RETURN_SIZE_HPP__

namespace luatic {

    namespace detail {

        template <class T>
        class ReturnSize {
        public:
            static const int Value = 1;
        };

        template <>
        class ReturnSize<void> {
        public:
            static const int Value = 0;
        };

        template <class... Ts>
        class ReturnSize<std::tuple<Ts...> > {
        public:
            static const int Value = std::tuple_size<std::tuple<Ts...> >::value;
        };
        
    } // namespace detail
    
} // namespace luatic

#endif // __LUATIC_RETURN_SIZE_HPP__
