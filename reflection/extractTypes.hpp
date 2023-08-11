#pragma once

#include <type_traits>

#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/remove_parens.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

#ifndef MAX_STRUCT_MEMBERS
#define MAX_STRUCT_MEMBERS 64
#endif

namespace {

template <class T, class... TArgs>
decltype(void(T{std::declval<TArgs>()...}), std::true_type{}) test_is_braces_constructible(int);
template <class, class...>
std::false_type test_is_braces_constructible(...);
template <class T, class... TArgs>
using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));

struct any_type {
  template<class T>
  constexpr operator T(); // non explicit
};

}

#define SELFINCR(z, n, r) BOOST_PP_COMMA_IF(n) r##n
#define TYPECELL(z, n, r) BOOST_PP_COMMA_IF(n) std::decay_t<decltype(r##n)>
#define ANY_TYPE(z, n, r) BOOST_PP_COMMA_IF(n) any_type
#define DCREPEAT(u, n, m, r) BOOST_PP_REPEAT(BOOST_PP_SUB(u, n), m, r)

#define BRANCH(z, n, _) if constexpr(is_braces_constructible<Type, DCREPEAT(MAX_STRUCT_MEMBERS, n, ANY_TYPE, _)>{}) { \
    auto& [ DCREPEAT(MAX_STRUCT_MEMBERS, n, SELFINCR, m) ] = *tptr; \
    return R<DCREPEAT(MAX_STRUCT_MEMBERS, n, TYPECELL, m) > (std::forward<Args>(args)...); \
} else

template<typename T, template <typename ...Unused> typename R, typename ...Args>
constexpr auto extractTypes(Args&&... args) noexcept {
    using Type = std::decay_t<T>;
    Type* tptr;
    BOOST_PP_REPEAT(MAX_STRUCT_MEMBERS, BRANCH, _) {
        return R<> {};
    }
    /*
     Expand to
     if constexpr(is_braces_constructible<type, any_type, ...>{}) {
        auto& [ m0, ... ] = *tptr;
        return R< std::decay_t<decltype(m0)>, ... > {};
     } else
         ...
         if constexpr(is_braces_constructible<type, any_type>{}) {
        auto&& [m0] = *tptr;
        return R< std::decay_t<decltype(m0)> > {};
     } else {
        return R<> {};
     }
    */
}

template<typename T, template <typename ...Unused> typename R>
using MemberTypes = decltype(extractTypes<T, R>());

#undef SELFINCR
#undef TYPECELL
#undef ANY_TYPE
#undef DCREPEAT
#undef MAX_STRUCT_MEMBERS
