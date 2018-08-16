
#include <cstddef>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

template <auto Value, template <typename...> class... __Policies>
struct X : __Policies<X<Value, __Policies...>>... {
  constexpr X() = default;
  constexpr X(__Policies<X> const &... f) : __Policies<X>(f)... {
    std::cout << "default ctor" << std::endl;
  }

  // template <typename... __Args>
  // X(__Args &&... args) : __Policies<X>{std::forward<__Args>(args)}... {
  //  std::cout << "perfect forwarding ctor" << std::endl;
  // }

  static inline constexpr auto value = Value;
};

template <typename Implementation, typename I = int>
struct A {
  A() = default;
  A(int a1) : a1_{a1} {}
  A(int a1, int a2) : a1_{a1}, a2_{a2} {}

  int a1_, a2_;
};

template <typename... __Implementation>
struct B {
  B() = default;
  B(std::string_view b) : b_{b} {}

  std::string b_;
};

template <typename Implementation>
class Stream_t {  // Barton-Nackman
  friend constexpr auto &operator<<(std::ostream &a, Implementation const &b) {
    return a << b.value;
  }
};

int main() {
  using X1 = X<1, A, B, Stream_t>;
  auto x1 = X1{};
  auto x2 = X<2, A, B>{{1, 2}, {std::string("test")}};
  // auto x3 = X<A, B>{1, 2, std::string("test")};      // Error
  std::cout << x1 << std::endl;
}
