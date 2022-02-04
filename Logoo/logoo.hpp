#pragma once
#include <cxxabi.h>

#include <cstdio>
#include <typeinfo>

#define BACKWARD_HAS_BFD 1
#include "backward.hpp"

template <typename T>
struct Simple{
    void copied(const T* lhs, const T* rhs){
        std::printf("copied");
    }

    void moved(const T* lhs, const T* rhs){
        std::printf("moved");
    }

    void destructed(const T* lhs){
        std::printf("destructed");
    }
};

template <typename T, std::size_t D = 6>
struct Trace{
    static void trace(){
        backward::StackTrace st;
        st.load_here(D);
        st.skip_n_firsts(D - 1);
		backward::Printer p;
        p.print(st);
    }

    void copied(const T* lhs, const T* rhs){
        trace();
        std::printf("copied");
    }

    void moved(const T* lhs, const T* rhs){
        trace();
        std::printf("moved");
    }

    void destructed(const T* lhs){
        trace();
        std::printf("destructed");
    }
};

template <typename T>
struct Stamped{
    Stamped(): id_(getId()){}
    Stamped(const Stamped& rhs): id_(rhs.id_){}
    Stamped(Stamped&& rhs): id_(rhs.id_){}

    char* type(){
        char* name = 0;
        int status;
        name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        return name;
    }

    void copied(const T* lhs, const T* rhs){
        char* name = type();
        std::printf("\033[32mCOPY \033[1m\033[34m%zx\033[0m->\033[1m\033[34m%zx \033[0m%s \033[1m\033[32mfingerprint: %zu\033[0m\n", reinterpret_cast<std::uintptr_t>(rhs), reinterpret_cast<std::uintptr_t>(lhs), name, id_);
        free(name);
    }

    void moved(const T* lhs, const T* rhs){
        char* name = type();
        std::printf("\033[1m\033[36mMOVE \033[34m%zx\033[0m->\033[1m\033[34m%zx \033[0m%s \033[1m\033[32mfingerprint: %zu\033[0m\n", reinterpret_cast<std::uintptr_t>(rhs), reinterpret_cast<std::uintptr_t>(lhs), name, id_);
        free(name);
    }

    void destructed(const T* lhs){
        char* name = type();
        std::printf("\033[1m\033[35mDESTRUCT \033[34m%zx \033[0m%s \033[1m\033[32mfingerprint: %zu\033[0m\n", reinterpret_cast<std::uintptr_t>(lhs), name, id_);
        free(name);
    }

  private:
    static std::size_t getId(){
        static std::size_t id = 0;
        return ++id;
    }

    const std::size_t id_;
};

template <typename T, std::size_t D = 3>
struct TraceStamped{
    TraceStamped(){}
    TraceStamped(const TraceStamped& rhs): stamped_(rhs.stamped_){}
    TraceStamped(TraceStamped&& rhs): stamped_(std::move(rhs.stamped_)){}

    void copied(const T* lhs, const T* rhs){
        stamped_.copied(lhs, rhs);
        trace_.trace();
        std::printf("\n");
    }

    void moved(const T* lhs, const T* rhs){
        stamped_.moved(lhs, rhs);
        trace_.trace();
        std::printf("\n");
    }

    void destructed(const T* lhs){
        stamped_.destructed(lhs);
        trace_.trace();
        std::printf("\n");
    }

  private:
    Stamped<T> stamped_;
    Trace<T, D> trace_;
};


template <typename T, template <typename U = T> class M = Stamped>
class Logoo: public T, public M<>{
  public:
    using T::T;

    Logoo(const Logoo& rhs): T(rhs), M<>(rhs){
        // Multiple inheritance cause M::this, T::this and Logoo::this may differ
        // Force to T::this to consolidate
        M<>::copied(&rhs, &static_cast<const T&>(*this));
    }

    Logoo(Logoo&& rhs): T(std::move(rhs)), M<>(std::move(rhs)){
        M<>::moved(&rhs, &static_cast<const T&>(*this));
    }

    Logoo(const T& rhs): T(rhs){
        M<>::copied(&rhs, &static_cast<const T&>(*this));
    }

    Logoo(T&& rhs): T(std::move(rhs)){
        M<>::moved(&rhs, &static_cast<const T&>(*this));
    }

    ~Logoo(){
        M<>::destructed(&static_cast<const T&>(*this));
    }
};