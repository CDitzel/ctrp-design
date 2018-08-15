#include <iostream>
#include <memory>
#include <vector>

/*
The curiously recurring template pattern enables the simulation of virtual calls
at compile time using meta-programming that eventually translates to functions
overloading. Two main purposes to employ the CRTP: Adding Functionality and
creating Static Interfaces. There is at least one argument for using the CRTP
over non-member template functions: the CRTP shows in the interface.
With the CRTP the situation is radically different. The derived class does not
express the fact it “is a” base class. Rather, it expands its interface
by inheriting from the base class, in order to add more functionality.
In this case it makes sense to use the derived class directly, and to never
use the base class (which is true for this usage of the CRTP, but not the one
described in the above Stack Overflow answer on static interfaces).
Therefore the base class is not the interface, and the derived class is not
the implementation. Rather, it is the other way around: the base class uses
the derived class methods (such as getValue and setValue). In this regard,
the derived class offers an interface to the base class.
This illustrates again the fact that inheritance in the context of the CRTP
can express quite a different thing from classical inheritance.
*/

// This class is merely for homogeneous use of derived instances in containers
struct Base {
  virtual ~Base() = default;
  virtual std::unique_ptr<Base> clone() const = 0;
};

// This class conceals the type, i.e. the template parameter of the CRTP
template<typename DerivedT, template<typename> class curiousType>
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

template<class DerivedT>
struct CuriousBase : crtp<DerivedT, CuriousBase> {

  std::unique_ptr<Base> clone() const override {
    return std::make_unique<DerivedT>(this->underlying());
  }

  void foo() const { this->underlying().bar(); }

  friend bool operator!=(DerivedT const &x1, DerivedT const &x2) {
    //return !(x1==x2);
    return x1.equalTo(x2);
  }

  auto operator+(const DerivedT &rhs) const {
    DerivedT lhs = static_cast<const DerivedT &>(*this);
    lhs += rhs;
    return lhs;
  }
  // protection against wrong chain of inheritance
 private:
  CuriousBase() = default;
  friend DerivedT;
};

template<typename CountedType>
class ObjectCounter {
 private:
  inline static std::size_t count = 0; // number of existing objects
 protected:
// default constructor
  ObjectCounter() {
    ++count;
  }
// copy constructor
  ObjectCounter(ObjectCounter<CountedType> const &) {
    ++count;
  }
// move constructor
  ObjectCounter(ObjectCounter<CountedType> &&) {
    ++count;
  }
// destructor
  ~ObjectCounter() {
    --count;

  }
 public:
// return number of existing objects:
  static std::size_t live() {
    return count;
  }
};
class ARS300 : public CuriousBase<ARS300> {
 public:
  void bar() const { std::cout << "ARS300 " << std::endl; }
  friend bool operator==(ARS300 const &x1, ARS300 const &x2) {
  }
  bool equalTo(ARS300 const &rhs) const {
    std::cerr << "ARS300 equalTo " << std::endl;
    return true;
  }
};

// Variadic curious crtp
// class X would have a basic interface but also augment them
// by inheriting from a set of CRTP base classes:
template<template<typename> typename... Features>
class X : public Features<X<Features...>> ... {
 public:
  void bar() const { std::cout << "X " << std::endl; }
};
template<typename T>
void invoke(CuriousBase<T> const &c) { c.foo(); }

int main() {
  auto _ars300 = ARS300{};
  invoke(_ars300);
  _ars300.bar(); // static interfaces without the need of virtual functions
  _ars300.foo(); // expanding the interface of the derived class

  auto _ars300_2 = ARS300{};
  auto &&_x = X<CuriousBase, ObjectCounter>{};
  auto &&_y = X<CuriousBase, ObjectCounter>{};
  _x.foo();

  _ars300 != _ars300_2;
  //_ars300 + _ars300_2;


  auto a = Array<int>{};
  auto b = a;
  a==b;

}


