#pragma once
#include "SI-lib.hpp"
#include <type_traits>
#include "radarPolicies.hpp"

namespace rf = radar::features;
namespace rt = radar::type;


template<typename T>
using EnableIfSizeGreater4 = std::enable_if_t<(sizeof(T) > 4)>;

enum class Layout {
  PolarGrid,
  RegularGrid
};

template<class _Identity, template<typename...> class ...__Features>
struct Radar : _Identity, __Features<Radar<_Identity, __Features...>> ... {

  constexpr explicit Radar(Layout layout) {
    std::cout << sizeof...(__Features) << std::endl;
  }

  //constexpr Radar() = default;
  // name injection
  constexpr explicit Radar(__Features<Radar> const &...);

  template<typename Concept = EnableIfSizeGreater4<_Identity>, typename...
  __Args>
  constexpr explicit Radar(__Args &&...);

  //static inline constexpr auto value = Value;

  constexpr auto Range() const;
};

template<class _Identity, template<typename...> class ...__Features>
constexpr Radar<_Identity, __Features...>::Radar(__Features<Radar> const &... f)
    :__Features<Radar>{f}... {
  puts(__PRETTY_FUNCTION__);
}

template<class _Identity, template<typename...> class ...__Features>
template<typename Concept, typename... __Args>
constexpr Radar<_Identity, __Features...>::Radar(__Args &&... args) :
    __Features<Radar>{std::forward<__Args>(args)...}... {
  // expands both the template parameter pack and the function parameter pack
  std::cout << "perfect forwarding ctor" << std::endl;
  puts(__PRETTY_FUNCTION__);
}

// out-of-class definition
template<class _Identity, template<typename...> class ...__Features>
constexpr auto Radar<_Identity, __Features...>::Range() const {
  std::clog << "Instance counter: " << this->live() << '\n';
  return _Identity::maxRange_; // to include names back into scope
};
