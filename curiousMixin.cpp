#include <iostream>
#include <string>

template <template <typename... __Implementation> typename... __Policies>
struct X : __Policies<X<__Policies...>>... {
  X() = default;
  X(__Policies<X> const &... f) : __Policies<X>(f)... {
    std::cout << "default ctor" << std::endl;
  }

  template <typename... __Args>
  X(__Args &&... args)
      : __Policies<X<__Policies...>>{std::forward<__Args>(args)}... {
    std::cout << "perfect forwarding ctor" << std::endl;
  }
};

template <typename Implementation>
struct A {
  A() = default;
  A(int a1) : a1_{a1} {}
  A(int a1, int a2) : a1_{a1}, a2_{a2} {}

  int a1_, a2_;
};

template <typename... __Implementation>
struct B {
  B() = default;
  B(std::string b) : b_{b} {}

  std::string b_;
};

int main() {
  auto x1 = X<A, B>{};  // Ok
  auto x2 = X<A, B>{{1, 2}, {std::string("test")}};
  // auto x3 = X<A, B>{1, 2, std::string("test")};      // Error
  std::cout << "Hi " << std::endl;
}
