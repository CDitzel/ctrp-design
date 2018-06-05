#pragma once
#include "tiffio.h"
#include <bitset>
#include <cassert>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace Tesseract {

template <typename T, typename... Ts>
void
emplace_back_All(std::vector<T>& vec, Ts... ts)
{ // "," prevents parameter pack from being folded
  (vec.emplace_back(ts), ...);
}

template <typename T>
static auto
emplace_back_All(std::vector<T>& vec)
{ // "," prevents parameter pack from being folded
  return [&](auto&&... ts) {
    (vec.emplace_back(std::forward<decltype(ts)>(ts)), ...);
  };
}

/*!
 * \brief emplace_All                   Emplaces multiple parameters into an
 * \param associativeContainer          Associative container of unique keys
 * \param ts                            Parameter pack, i.e. value-type of
 *                                      either std::set or std::map
 * \return                              True if all emplacements succeeded
 */
template <typename T, typename... Ts>
bool
emplace_All(T& associativeContainer, Ts... ts)
{
  return (associativeContainer.emplace(ts).second && ...);
}
// use like so:
// std::pair<int, int> s{ 3, 5 };
// if (emplace_All(m_, std::make_pair(1, 2), s, std::pair{ 4, 3 }))

template <typename... Ts>
void
obliterate(Ts&... ts)
{
  (ts.clear(), ...);
}

/*!
 * \brief shell_all                     Writes all elements to standard output
 * \param container                     Arbitrary container, its value type has
 *                                      to have implemented operator<<()
 * \param os                            Arbitrary subclass of std::ostream
 *
 */
template <typename T>
void
shell_all(const T& container, std::ostream& os = std::cout)
{
  os.setf(std::ios::left | std::ios::adjustfield | std::ios::unitbuf);
  os << "-- Shell all --\n";
  std::copy(std::cbegin(container), std::cend(container),
            std::ostream_iterator<typename T::value_type>{ os, "\n" });
}

/*!
 * \brief shell_it                      Allows for formatted output of any
 * \param obj                           Object that defined an output operator
 */
template <typename T>
void
shell_it(const T& obj)
{ // almost always auto
  auto sheller = std::ostream_iterator<T>{ std::cout, "\n" };
  sheller = obj;
}

/*!
 * \brief scaleValue        scales the given value accordings to the parameters
 * \param min               minimum value of the range to be transformed and the
 * \param max               maximum value of the range to be transformed
 * \param new_max           a new maximum for the projection to the range, e.g.
 *                          new_max = 1 ranges the value in the unit intervall
 */
static auto
scaleValue(float min, float max, float new_max = 1)
{
  const float diff{ max - min };
  assert(diff != 0); // prevent division by zero
  return [=](float val) -> float { return (val - min) / diff * new_max; };
}

/*!
 * \brief clampValue        this function object builder limits values to the
 * \param min               specified minimum cut-off number and the
 * \param max               specified maximum cut-off number
 */
static auto
clampValue(float min, float max)
{
  return [=](float val) -> float { return std::clamp(val, min, max); };
}

/*!
 * \brief multiCall         fire-and-forget function that compiles with a
 * \param functions         collection of function objects that are called
 *                          regardless of whether they return anything or what
 *                          they return. It has a variadic constructor while the
 *                          parameter pack is expanded with an initializer list
 *                          and the functions return value is thrown away and
 *                          0 is inserted eventually
 */
static auto
multiCall(auto&... functions)
{
  return [&](auto const& arg) {
    static_cast<void>(
      std::initializer_list<int>{ (static_cast<void>(functions(arg)), 0)... });
  };
}

static auto
for_each_param(auto f, auto... xs)
{
  (void)std::initializer_list<int>{ ((void)f(xs), 0)... };
}

/*!
 * \brief within            Closure builder that returns a function that
 *                          checks if any object implementing operator<() is
 * \param min               within bounds given by the minumum and maximum
 * \param max               value used while construction of the lambda
 */
template <typename T>
[[maybe_unused]] static auto
within(T min, T max)
{
  return [=](T val) {
    //    std::cerr << "min " << min << "max " << max << "val" << val <<
    //   std::endl;
    //    if (min <= val && val <= max)
    //      std::cerr << "SUCCESS " << std::endl;
    return min <= val && val <= max;
  };
}

/*!
 * \brief allWithin         Checks whether a number of objects are within range
 * \param min               defined by a minimum and a
 * \param max               maximum respectively
 * \param ts                Parameter pack that is to be checked for inclusion
 * \return                  True if all passed arguments are within bounds
 */
template <typename T, typename... Ts>
[[nodiscard]] bool
allWithin(T min, T max, Ts... ts)
{
  return ((min <= ts && ts <= max) && ...);
}

/*!
 * \brief combine           Creates complex predicates with logical conjunction
 * \param binary_func       Binary function, e.g. logical AND, logical OR, etc.
 * \param a                 Predicate function, i.e. function returning a bool
 * \param b                 Second predicate function that is combined with a
 */
template <typename F, typename A, typename B>
[[nodiscard]] auto
combine(F binary_func, A predicateA, B predicateB)
{
  return [=](auto param) {
    return binary_func(predicateA(param), predicateB(param));
  };
}
// use like so:
// static bool begins_with_a(const std::string &s){return s.find("a") == 0;}
// static bool ends_with_b(const std::string &s){return s.rfind("b") ==
// s.length() -1;}
// auto a_xxx_b {combine(logical_and<>{}, begins_with_a, ends_with_b);};
// std::copy_if(ifstream_iterator<std::string>{cin}, {},
// ostream_iterator<std::string>{std::cout, ","}, a_xxx_b);
// $ echo "ac cb ab axxb" | ./combine
// ab, axxxb,

/*!
 * \brief opt_print         Function builder that encapsulates the end iterator
 *                          of a data structure
 * \param container         Arbitrary container conforming to the STL standard
 */
template <typename C>
[[nodiscard]] static auto
safe_deref(const C& container)
{
  return [end_it{ std::cend(container) }](const auto& iter)
    ->std::optional<typename C::value_type>
  {
    if (iter != end_it)
      return *iter;
    else
      return std::nullopt;
  };
}

/*!
 * \brief insertionSort             In just two lines of code
 * \param start
 * \param end
 */
template <typename T>
void
insertionSort(T start, T end)
{
  for (auto i = start; i != end; ++i)
    std::rotate(std::upper_bound(start, i, *i), i, std::next(i));
}

template <class FwdIt, class Compare = std::less<>>
void
quickSort(FwdIt first, FwdIt last, Compare cmp = Compare{})
{
  auto const N{ std::distance(first, last) };
  if (N <= 1)
    return;
  auto const pivot{ std::next(first, N / 2) };
  std::nth_element(first, pivot, last, cmp);
  quickSort(first, pivot, cmp); // assert(std::is_sorted(first, pivot, cmp));
  quickSort(pivot, last, cmp);  // assert(std::is_sorted(pivot, last, cmp));

  // can be optimized by adding insertionSort call for small ranges
}

template <typename I, typename P>
auto
stable_partition(I f, I l, P p) -> I
{
  auto n{ std::distance(l - f) };
  if (n == 0)
    return f;
  if (n == 1)
    return f + p(*f);
  auto m{ f + (n / 2) };
  return std::rotate(stable_partition(f, m, p), m, stable_partition(m, l, p));
}

// use case: list of items, make continous selection and move that selection
// into a new 'p' position.
template <typename randIter>
auto
slide(randIter f, randIter l, randIter p) -> std::pair<randIter, randIter>
{
  if (p < f)
    return { p, std::rotate(p, f, l) };
  if (l < p)
    return { std::rotate(f, l, p), p };
  return { f, l };
}

// use case: list of items, select some of items (good guys) and move the to
// position around p.
// for instance: multiple selection on a list
template <typename I, // I models BidirectionalIterator
          typename S> // S models UnaryPredicate
auto
gather(I f, I l, I p, S s) -> std::pair<I, I>
{
  using value_type = typename std::iterator_traits<I>::value_type;
  return { std::stable_partition(f, p,
                                 [&](const value_type& x) { return !s(x); }),
           std::stable_partition(p, l, s) };
}

// trim for strings
// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
static std::string
trimLeft(const std::string& s)
{
  auto temp = s;
  temp.erase(std::begin(temp),
             std::find_if(std::begin(temp), std::end(temp), [](char c) {
               return !std::isspace(c, std::locale());
             }));
  return temp;
}

/*!
 * Transform_if implementation, TODO: A lot
 */
template <typename T>
auto
map(T fn)
{
  return [=](auto reduce_fn) {
    return [=](auto accum, auto input) { return reduce_fn(accum, fn(input)); };
  };
}

template <typename T>
auto
filter(T predicate)
{
  return [=](auto accum, auto input) {
    if (predicate(input)) {
      return reduce_fn(accum, input);
    } else {
      return accum;
    }
  };
};

auto copy_and_advance{ [](auto it, auto input) {
  *it = input;
  return ++it;
} };

}
