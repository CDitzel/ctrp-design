#pragma once

using namespace si;

namespace radar::features {

template<typename... __Implementation>
struct B {
  B() = default;
  B(std::string_view b) : b_{b} {}

  std::string b_;
};

template<typename _Impl>
class Diagnosis_t {  // Barton-Nackman
  friend constexpr auto &operator<<(std::ostream &os, _Impl const &_impl) {
    return os << std::scientific << "reachable?" << _impl.renderGrid() <<
             std::endl;
  }
};

template<typename CountedType, typename I = std::size_t>
class ObjectCounter {

  using self_type = ObjectCounter<CountedType, I>;

  inline static I count = 0; // number of existing objects
 public:
// default constructor
  ObjectCounter() {
    ++count;
    std::cout << "Base class ctor" << std::endl;
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
// return number of existing objects:
  static auto live() {
    return count;
  }
};

template<typename T>
struct crtp {
  T &underlying() { return static_cast<T &>(*this); }
  T const &underlying() const { return static_cast<T const &>(*this); }
};

template<typename _Interface>
struct Grid : crtp<_Interface> {
  auto renderGrid() const {
    return static_cast<_Interface const &>(*this).Range();
  }
};

} // namespace radar::features

namespace radar::type {
struct ARS300 {
  Length const maxRange_ = 200.0_m;
  Frequency const carrierFrequency_ = 77000000000.0_Hz;
  Frequency const bandwidth_ = 20000000000.0_Hz;
  int const numberOfRangeCells = 200;
  int const numberOfAngularBeams = 17;
};

struct Inras {
  Length const maxRange_ = 50.0_m;
  Frequency const carrierFrequency_ = 77000000000.0_Hz;
  Frequency const bandwidth_ = 20000000000.0_Hz;
  int const numberOfRangeCells = 100;
  int const numberOfAngularCells = 100;
};

// Primary variable template
template<typename T>
constexpr auto isAutomotiveRadar = false;
// Full specialization
template<>
constexpr auto isAutomotiveRadar<ARS300> = true;
} // namespace radar::type
