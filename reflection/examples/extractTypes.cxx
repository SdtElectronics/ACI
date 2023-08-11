#include <iostream>
#include "extractTypes.hpp"

struct S { };

struct S1 {
    double c;
    char j;
    S s;
};

template <typename T>
struct TypeName {
    static constexpr const char* name = "unknown ";
};

template <>
struct TypeName<double> {
    static constexpr const char* name = "double ";
};

template <>
struct TypeName<char> {
    static constexpr const char* name = "char ";
};

template <>
struct TypeName<S> {
    static constexpr const char* name = "S ";
};

template <typename ...Types>
struct TypeSeq {
    static void print() {
        (std::cout << ... << TypeName<Types>::name ) << std::endl;
    }
};

int main() {
    MemberTypes<S1, TypeSeq>::print();
}
