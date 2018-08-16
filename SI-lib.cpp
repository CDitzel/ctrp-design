#include <string>

namespace si {
// Compile-time type-safety
template <int M, int K, int S> struct MksUnit {
  enum { metre = M, kilogram = K, second = S };
};

template <typename> class Value {
public:
  constexpr explicit Value(const long double magnitude) noexcept
      : magnitude_{magnitude} {}
  constexpr long double magnitude() const noexcept { return magnitude_; }
  explicit operator long double() const { return magnitude_; }

private:
  long double const magnitude_{0.0};
};

using DimensionlessQuantity = Value<MksUnit<0, 0, 0>>;
using Length = Value<MksUnit<1, 0, 0>>;
using Area = Value<MksUnit<2, 0, 0>>;
using Volume = Value<MksUnit<3, 0, 0>>;
using Mass = Value<MksUnit<0, 1, 0>>;
using Time = Value<MksUnit<0, 0, 1>>;
using Speed = Value<MksUnit<1, 0, -1>>;
using Acceleration = Value<MksUnit<1, 0, -2>>;
using Frequency = Value<MksUnit<0, 0, -1>>;
using Force = Value<MksUnit<1, 1, -2>>;
using Pressure = Value<MksUnit<-1, 1, -2>>;
using Momentum = Value<MksUnit<1, 1, -1>>;

// A couple of convenient factory functions
constexpr auto operator"" _N(long double magnitude) { return Force(magnitude); }
constexpr auto operator"" _ms2(long double magnitude) {
  return Acceleration{magnitude};
}
constexpr auto operator"" _s(long double magnitude) { return Time(magnitude); }
constexpr auto operator"" _Ns(long double magnitude) {
  return Momentum(magnitude);
}
constexpr auto operator"" _m(long double magnitude) {
  return Length(magnitude);
}
constexpr auto operator"" _ms(long double magnitude) {
  return Speed(magnitude);
}
constexpr auto operator"" _kg(long double magnitude) { return Mass(magnitude); }

// Arithmetic operators for consistent type-rich conversions of SI-Units
template <int M, int K, int S>
constexpr auto operator+(Value<MksUnit<M, K, S>> const &lhs,
                         Value<MksUnit<M, K, S>> const &rhs) noexcept {
  return Value<MksUnit<M, K, S>>{static_cast<long double>(lhs) +
                                 static_cast<long double>(rhs)};
}

template <int M, int K, int S>
constexpr auto operator-(Value<MksUnit<M, K, S>> const &lhs,
                         Value<MksUnit<M, K, S>> const &rhs) noexcept {
  return Value<MksUnit<M, K, S>>{static_cast<long double>(lhs) -
                                 static_cast<long double>(rhs)};
}

template <int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator*(Value<MksUnit<M1, K1, S1>> const &lhs,
                         Value<MksUnit<M2, K2, S2>> const &rhs) noexcept {
  return Value<MksUnit<M1 + M2, K1 + K2, S1 + S2>>{
      static_cast<long double>(lhs) * static_cast<long double>(rhs)};
}

template <int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator/(Value<MksUnit<M1, K1, S1>> const &lhs,
                         Value<MksUnit<M2, K2, S2>> const &rhs) noexcept {
  return Value<MksUnit<M1 - M2, K1 - K2, S1 - S2>>{
      static_cast<long double>(lhs) / static_cast<long double>(rhs)};
}

// Scientific constants
auto constexpr speedOfLight = 299792458.0_ms;
auto constexpr gravitationalAccelerationOnEarth = 9.80665_ms2;

void applyMomentumToSpacecraftBody(Momentum const &impulseValue){};
} // namespace si
