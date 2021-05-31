#include <iostream>
#include <memory>
#include <vector>

/*
The curiously recurring template pattern enables the simulation of virtual calls
at compile time using metaprogramming that eventually translates to functions
overloading. Two main purposes to employ the CRTP: Adding Functionality and
creating Static Interfaces.
*/

// This class is merely for homogeneous use of dervied instances in containers
struct Base {
  virtual ~Base() = default;
  virtual std::unique_ptr<Base> clone() const = 0;
};

// This class conceals the type, i.e. the template parameter of the CRTP
template <typename DerivedT, template <typename> class curiousType>
struct crtp : Base {
  auto const &underlying() const {
    return static_cast<DerivedT const &>(*this);
  }
  auto &underlying() { return static_cast<DerivedT &>(*this); }

  // protection against wrong chain of inheritance
private:
  crtp() = default;
  friend curiousType<DerivedT>;
};

template <class DerivedT> struct CuriousBase : crtp<DerivedT, CuriousBase> {

  std::unique_ptr<Base> clone() const override {
    return std::make_unique<DerivedT>(this->underlying());
  }

  void foo() const { this->underlying().bar(); }

  // protection against wrong chain of inheritance
private:
  CuriousBase() = default;
  friend DerivedT;
};

// expands its interface by inherting from the base class in order to add more
// functionality
class ARS300 : public CuriousBase<ARS300> {
private:
  friend class CuriousBase<ARS300>;

  void bar() const { std::cout << "ARS300 " << std::endl; }
};

struct Inras : CuriousBase<Inras> {
  void bar() const { std::cout << "Inras " << std::endl; }
};

template <typename T> void invoke(CuriousBase<T> const &c) { c.foo(); }


int main() {
  // auto radars = std::vector<std::unique_ptr<Base>>{};
  // radars.emplace_back(std::make_unique<ARS300>());

  auto const ars300 = ARS300{};
  auto inras = Inras{};
  invoke(ars300);
  invoke(inras);

  if (true) {
    std::vector<int> v(10);
    auto iter = std::begin(v);
  }

}
