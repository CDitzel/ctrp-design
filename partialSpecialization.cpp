#include <functional>
#include <iostream>
#include <memory>
#include <vector>

template <typename T> struct Stack {
  T t_;
  Stack() { std::cout << "Templated Stack" << std::endl; }
  auto get() const { return t_; }
  void set();
  friend std::ostream &operator<<(std::ostream &os, Stack<T> const &s) {
    return os << "friend: " << s.t_ << std::endl;
  }
};

// Out-of-class definition of member function in a class template
template <typename T> void Stack<T>::set() {}

// Specialization of only a certain member function of a class template
template <> void Stack<float>::set() {
  std::clog << "float specialization" << std::endl;
}

// Complete specialzation of class template for type int (not a template
// anymore, but a type)
template <> struct Stack<int> {
  Stack() {
    std::cout << "Full Int specialization with additional data members"
              << std::endl;
  }
  std::vector<int> vec_;
  auto get() { return std::size(vec_); }
};

// Partial specialization of class template for const types
// Any specialization that is, itself, a template. It still requires further
// "customization" by the user before it can be used
template <typename T> struct Stack<T const> {
  Stack() { std::cout << "Partial specialization" << std::endl; }
};

struct Widget {
  // class with member function template
  template <typename U> U read();
};

// Out-of-class definition of member function template of a normal class
template <typename U> U Widget::read() {
  std::clog << "generic read" << std::endl;
}

// Out-of-class specialization of member function template
template <> int Widget::read() { std::clog << "int read" << std::endl; }

// free function template
template <typename T> T absolute(T x) {
  puts(__PRETTY_FUNCTION__);
  return x >= 0 ? x : -x;
}

// specialization of function template for int, explicit type can be deduced and
// may be left out. No function overloading kicks in, cuz these are not function
// but templates for functions
// template <> int absolute<int>(int x) {
// template <> int absolute<>(int x) {
template <> int absolute(int x) {
  puts(__PRETTY_FUNCTION__);
  if (!x == 0)
    return (x >= 0) ? x : -x;
}

// Alias templates (cannot be specialized as of now)
template <typename T> using myVec = std::vector<T>;

template <typename T> void foo(myVec<T> const &mv) {
  puts(__PRETTY_FUNCTION__);
}

int main() {
  Stack<int> intStack_;
  Stack<double> doubleStack_;
  int i;
  Stack<int const> refStack_;
  Stack<int *> ptrStack_;
  Stack<decltype(5.f)> val;
  auto w = Widget{};
  w.read<int>();
  w.read<double>();
  Stack<float> floatStack;
  floatStack.set();

  
  absolute(5.f);
  absolute(3);

  std::vector<int> v;
  foo(v);
  foo(std::vector<float>{});
}
