template<int M, int K, int S>
struct MksUnit {
  enum { metre = M, kilogram = K, second = S };
};

template<typename MksUnit>
class Value {
 public:
  constexpr explicit Value(const long double magnitude) noexcept : magnitude{
      magnitude} {}
  constexpr long double getMagnitude() const noexcept {
    return magnitude;
  }
 private:
  long double magnitude{0.0};
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

class SpacecraftTrajectoryControl {
 public:
  void applyMomentumToSpacecraftBody(Momentum const &impulseValue) {};
};

constexpr Acceleration gravitationalAccelerationOnEarth{9.80665};
constexpr Pressure standardPressureOnSeaLevel{1013.25};
constexpr Frequency concertPitchA{440.0};
constexpr Mass neutronMass{1.6749286e-27};
auto constexpr speedOfLight = Speed{299792458.0};

template<int M, int K, int S>
constexpr Value<MksUnit<M, K, S>> operator+
    (const auto &lhs,
     const Value<MksUnit<M, K, S>> &rhs) noexcept {
  return {lhs.getMagnitude() + rhs.getMagnitude()};
}

template<int M, int K, int S>
constexpr Value<MksUnit<M, K, S>> operator-
    (const Value<MksUnit<M, K, S>> &lhs,
     const Value<MksUnit<M, K, S>> &rhs) noexcept {
  return Value<MksUnit<M, K, S>>(lhs.getMagnitude() - rhs.getMagnitude());
}

template<int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator*
    (const Value<MksUnit<M1, K1, S1>> &lhs,
     const Value<MksUnit<M2, K2, S2>> &rhs) noexcept {
  return Value<MksUnit<M1 + M2, K1 + K2, S1 + S2>>{
      lhs.getMagnitude() * rhs.getMagnitude()};
}

template<int M1, int K1, int S1, int M2, int K2, int S2>
constexpr auto operator/
    (const Value<MksUnit<M1, K1, S1>> &lhs,
     const Value<MksUnit<M2, K2, S2>> &rhs) noexcept {
  return Value<MksUnit<M1 - M2, K1 - K2, S1 - S2>>(
      lhs.getMagnitude() / rhs.getMagnitude());
}

/*
It’s pretty expressive. And it’s safer, because you will not
be able to assign the result of the multiplication to something different
than a variable of type Momentum.
the type safety is ensured during compile time! There is no overhead during
runtime, because a C++11 (and higher)-compliant compiler can perform all
necessary type compatibility checks.*/

/*Since C++11, developers can produce objects of user-defined types by
 * defining user-defined suffixes for literals. Basically factory functions */
constexpr auto operator "" _N(long double magnitude) {
  return Force(magnitude);
}
constexpr auto operator "" _ms2(long double magnitude) {
  return Acceleration{magnitude};
}
constexpr auto operator "" _s(long double magnitude) {
  return Time(magnitude);
}
constexpr auto operator "" _Ns(long double magnitude) {
  return Momentum(magnitude);
}

/*
This notation is not only familiar to physicists and other scientists. It is even safer. With type-rich
    programming and user-defined literals you are protected against assigning a literal expressing a value of
seconds to a variable of type Force.
*/
Acceleration gravitationalAccelerationOnEarth2 = operator ""_ms2(80665);
auto constexpr gravitationalAccelerationOnEarth3 = 9.80665_ms2;

int main() {

  auto impulseValueForCourseCorrection = Force{30.0} * Time{3.0};
  auto impulseValueForCourseCorrection2 = Momentum{Force{30.0} * Time{3.0}};
  auto momentum = 9.81_N * 3.0_s;
  Momentum m = momentum;

  auto control = SpacecraftTrajectoryControl{};
  control.applyMomentumToSpacecraftBody(impulseValueForCourseCorrection);
}

