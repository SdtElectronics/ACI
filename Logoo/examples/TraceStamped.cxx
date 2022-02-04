#include <memory>
#include "logoo.hpp"

template <typename T>
using TraceStamped6 = TraceStamped<T, 6>;

int main(){
    using str = Logoo<std::string, TraceStamped6>;
    str s1(std::string{"a"});
    std::string st = s1 + "b";
    str s2 = st;
    
    using unique_ptr = Logoo<std::unique_ptr<int>, TraceStamped6>;
    unique_ptr p1 = std::make_unique<int>(1);
    unique_ptr p2 = std::move(p1);
}