#include <type_traits>

template<typename Value>
struct OperatorFacade {
  friend constexpr bool operator!=(Value const &lhs, Value const &rhs)
  noexcept {
    return !(lhs==rhs);
  }
  friend constexpr bool operator>(Value const &lhs, Value const &rhs) noexcept {
    return rhs < lhs;
  }
  friend constexpr bool operator<=(Value const &lhs, Value const &rhs)
  noexcept {
    return !(rhs > lhs);
  }
  friend constexpr bool operator>=(Value const &lhs, Value const &rhs)
  noexcept {
    return !(rhs < lhs);
  }
  friend constexpr auto &operator<<(std::ostream &os, Value const &other)
  noexcept {
    std::cerr << other.magnitude_ << std::endl;
    return os << static_cast<long double>(other);
  }
  friend constexpr auto operator-(Value const &lhs,
                                  Value const &rhs) noexcept {
    //return Value{lhs} -= rhs;
    return Value{static_cast<long double>(lhs) - static_cast<long double>(rhs)};
  }
  friend constexpr auto operator+(Value const &lhs,
                                  Value const &rhs) noexcept {
    return Value{
        static_cast<long double>(lhs) + static_cast<long double>(rhs)};
  }
};

// Type-safety at compile-time
template<auto M = 0, auto K = 0, auto S = 0>
struct MksUnit {
  enum { metre = M, kilogram = K, second = S };
};

template<typename U = MksUnit<>> // default to dimensionless value
class Value final : public OperatorFacade<Value<U>> {
 public:
// TODO: fix special member functions
#if 0
  Value(Value const &other) : OperatorFacade<Value>(other) {
    std::cerr << "Copy ctor" << std::endl;
  }
  Value(Value &&other) noexcept : OperatorFacade<Value>(std::move(other)) {
    std::cerr << "Move ctor" << std::endl;
  }
  auto &operator=(Value const &other) {
    std::cerr << "copy assign" << std::endl;
    magnitude_ = other.magnitude_;
    return *this;
  }
  auto &operator=(Value &&other) noexcept {
    std::cerr << "Move assign " << std::endl;
    magnitude_ = std::move(other.magnitude_);
    return *this;
  }
#endif
  constexpr explicit Value() noexcept = default;
  constexpr explicit Value(long double const &magnitude) noexcept
      : magnitude_{magnitude} {}
  //constexpr auto &magnitude()  noexcept { return magnitude_; }
  constexpr explicit operator long double() const noexcept {
    return magnitude_;
  }

  friend constexpr bool operator==(Value const &lhs, Value const &rhs)noexcept {
    return lhs.magnitude_==rhs.magnitude_;
  }
  friend constexpr bool operator<(Value const &lhs, Value const &rhs)noexcept {
    return lhs.magnitude_ < rhs.magnitude_;
  }
  constexpr auto &operator+=(Value const &other) noexcept {
    magnitude_ += other.magnitude_;
    return *this;
  }
  constexpr auto &operator-=(Value const &other) noexcept {
    magnitude_ -= other.magnitude_;
    return *this;
  }
  friend constexpr auto operator*(long double const &scalar, Value const &other)
  noexcept {
    return other*scalar;
  }
  constexpr auto operator*(long double const &scalar) const noexcept {
    return Value{magnitude_*scalar};
  }
  constexpr auto &operator*=(long double const &scalar) noexcept {
    magnitude_ *= scalar;
    return *this;
  }
  constexpr auto &operator/=(long double const &scalar) noexcept {
    magnitude_ /= scalar;
    return *this;
  }
  //private:
  long double magnitude_ = 0L;
};

// Some handy alias declarations
using DimensionlessQuantity = Value<>;
using Length = Value<MksUnit<1, 0, 0>>;
using Mass = Value<MksUnit<0, 1, 0>>;
using Time = Value<MksUnit<0, 0, 1>>;
using Velocity = Value<MksUnit<1, 0, -1>>;
using Acceleration = Value<MksUnit<1, 0, -2>>;
using Area = Value<MksUnit<2, 0, 0>>;
using Volume = Value<MksUnit<3, 0, 0>>;
using Frequency = Value<MksUnit<0, 0, -1>>;
using Force = Value<MksUnit<1, 1, -2>>;
using Pressure = Value<MksUnit<-1, 1, -2>>;
using Momentum = Value<MksUnit<1, 1, -1>>;
using Work = Value<MksUnit<2, 1, -2>>;
using Power = Value<MksUnit<2, 1, -3>>;
using Ampere = Value<MksUnit<2, 1, -3>>;
using Temperature = Value<MksUnit<2, 1, -3>>;
using Luminosity = Value<MksUnit<2, 1, -3>>;

namespace si {
// A couple of convenient factory functions
constexpr auto operator "" _N(long double magnitude) noexcept {
  return Force{magnitude};
}
constexpr auto operator "" _ms2(long double magnitude)noexcept {
  return Acceleration{magnitude};
}
constexpr auto operator "" _s(long double magnitude) noexcept {
  return Time{magnitude};
}
constexpr auto operator "" _Ns(long double magnitude)noexcept {
  return Momentum{magnitude};
}
constexpr auto operator "" _m(long double magnitude)noexcept {
  return Length{magnitude};
}
constexpr auto operator "" _ms(long double magnitude)noexcept {
  return Velocity{magnitude};
}
constexpr auto operator "" _kg(long double magnitude)noexcept {
  return Mass{magnitude};
}
constexpr auto operator "" _Hz(long double magnitude)noexcept {
  return Frequency{magnitude};
}
constexpr auto operator "" _Nm(long double magnitude)noexcept {
  return Work{magnitude};
}
constexpr auto operator "" _W(long double magnitude)noexcept {
  return Power{magnitude};
}
// Scientific constants
auto constexpr speedOfLight = 299792458.0_ms;
auto constexpr gravitationalAccelerationOnEarth = 9.80665_ms2;
}
// Arithmetic operators for consistent type-rich conversions of SI-Units
template<int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator*(Value<MksUnit<M1, K1, S1>> const &lhs,
                         Value<MksUnit<M2, K2, S2>> const &rhs) noexcept {
  return Value<MksUnit<M1 + M2, K1 + K2, S1 + S2>>{
      static_cast<long double>(lhs)*static_cast<long double>(rhs)};
}

template<int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator/(Value<MksUnit<M1, K1, S1>> const &lhs,
                         Value<MksUnit<M2, K2, S2>> const &rhs) noexcept {
  return Value<MksUnit<M1 - M2, K1 - K2, S1 - S2>>{
      static_cast<long double>(lhs)/static_cast<long double>(rhs)};
}

void applyMomentumToSpacecraftBody(Momentum const &impulseValue) {};


