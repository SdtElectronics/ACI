# Track the Lifetime of C++ Objects: The Log of Object Inspector
Blogpost discussing the motivation and implementation in detail: [Track Lifetime of Objects in C++](https://sdtelectronics.github.io/programming/2022/02/02/Track-Lifetime-of-Objects-in-C++.html)
## Prerequisite
`binutils-dev` (`binutils` for Arch-family distros) if stack trace is enabled.
## Usage
Logoo is a header-only utility. Copy `logoo.hpp` and `backward.hpp` to the include path of your project, and include `logoo.hpp` to use. Substitute the type to be traced with `Logoo<T, M>`, where `T` is that type and `M` is the class designating the callbacks. Logoo has implemented 4 types to be used as `M`:
* `Simple`: Prints "copied", "moved" and "destructed" when corresponding event occurs
* `Trace`: Added stack trace for each events. Requires `binutils-dev`
* `Stamped`: Record the addresses and fingerprints of objects
* `TraceStamped`: Combination of `Trace` and `Stamped`

`M` is defaulted to `Stamped` when not provided.
## Examples
See [examples/](examples/). Run `make` in `./` to build.
## Implement your own callbacks
The layout of the class to be used as `M` must comply with the following requirement:
``` c++
// T is the type to be traced
template <typename T>
struct Callbacks{
    // lhs is the destination and rhs is the source
    void copied(const T* lhs, const T* rhs){
        // ...
    }
    // lhs is the destination and rhs is the source
    void moved(const T* lhs, const T* rhs){
        // ...
    }
    // lhs is the address of the object to be destructed
    void destructed(const T* lhs){
        // ...
    }
};
```