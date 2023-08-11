# Static Reflection Facilities

Facilities for compile-time reflection. Code in this directory requires C++17 or above to compile.

## extractTypes.hpp

### Synopsis
``` c++
#include "extractTypes.hpp"

#define MAX_STRUCT_MEMBERS 64

template<typename T, template <typename ...Unused> typename R, typename ...Args>
constexpr auto extractTypes(Args... args) noexcept;

template<typename T, template <typename ...Unused> typename R>
using MemberTypes = decltype(extractTypes<T, R>());
```

### Description
`extractTypes()` maps an aggregate-initializable structure type `T` to a user-defined structure `R` templated with variadic types the same as members in `T`. An optional set of arguments `...args` can be provided to feed the constructor of `R`.

`MemberTypes` is the type of the structure `R` that would be returned by `extractTypes()` if it's called with the same template arguments. If `R` is a structure with no non-static members, this can be used as a shortcut to directly get the type of `R`, which is what really matters. Note if `R` has user-defined constructors，a constructor with no parameters must be provided to make `MemberTypes<T, R>` well-formed.

`MAX_STRUCT_MEMBERS` defines the maximum number of members that the struct `T` may have. It's defaulted to 64, but user may designate a greater value if needed, at the expense of longer compile times.

### Dependencies
In addition to a compiler with support to C++17 or above, the boost library is required.

### Example
``` c++
#include "extractTypes.hpp"
struct S1 {
    int c;
    char j;
};

template <typename ...Types>
struct TypeSeq {
    /* Do stuffs with the extracted types*/
};

auto typeSeq = extractTypes<S1, TypeSeq>(); /* TypeSeq<int, char> {} */
using Extracted = MemberTypes<S1, TypeSeq>; /* TypeSeq<int, char> */
```
