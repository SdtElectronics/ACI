#include <memory>
#include "logoo.hpp"

int main(){
    using str = Logoo<std::string>;
    str s1(std::string{"a"});
    std::string st = s1 + "b";
    str s2 = st;
    
    using unique_ptr = Logoo<std::unique_ptr<int> >;
    unique_ptr p1 = std::make_unique<int>(1);
    unique_ptr p2 = std::move(p1);
}