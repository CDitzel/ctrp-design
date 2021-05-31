#include <iostream>
#include "radar.hpp"
#include "radarPolicies.hpp"

using namespace radar::features;
using namespace radar::type;

# if 0
template<typename T>
void invoke(CuriousBase<T> const &c) { c.foo(); }

#endif
template<typename T>
using myGrid = Grid<T>;

int main() {
  // invoke(_ars300);
  using ARS300Model = Radar<ARS300, ObjectCounter, Diagnosis_t, myGrid >;
  auto ars300Model = ARS300Model{Layout::PolarGrid};
  if (rt::isAutomotiveRadar<rt::ARS300>)
    std::cout << ars300Model << std::endl;
}





