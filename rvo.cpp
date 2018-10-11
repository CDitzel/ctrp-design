// g++ test.cpp -std=c++14 -fno-elide-constructors -O0

#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

struct Widget {
  Widget() { std::cerr << "Default ctor" << std::endl; }
  explicit Widget(int i) : i_{i} { std::cerr << "custom ctor" << std::endl; }
  Widget(Widget const &) { std::cerr << "copy ctor" << std::endl; }
  Widget(Widget &&) { std::cerr << "move ctor" << std::endl; }
  Widget &operator=(Widget const &) { std::cerr << "copy assign" << std::endl; }
  Widget &operator=(Widget &&) { std::cerr << "move assign" << std::endl; }
  int i_ = 7;
};

auto rvoWidget() {
  auto w = Widget{};
  w.i_ = 1;
  return w;
}

auto rvoPair() {
  // auto p = std::pair<Widget, Widget>(1,2);
  auto p1 = std::pair<Widget, Widget>{1, 2};
  // auto p2 = std::pair{Widget{1},Widget{2}};
  auto &[w1, w2] = p1;
  std::cerr << w1.i_ << " " << w2.i_ << std::endl;
  return p1;
}

// auto as return type does not work here due to different return types
// (std::nullopt) also causes this to do a move instead of copy elision
std::optional<Widget> rvoOpt() {
  // First alternative
   auto o = std::make_optional<Widget>();
   // Second alternative
  //auto o = std::optional<Widget>{};
  //o.emplace();
  o->i_ = 5;
  return o;
}

using C = std::pair<std::shared_ptr<Widget>, Widget>;

std::optional<C> rvoOptPair() {
  // auto o = std::make_optional<C>();
  // auto o = std::optional<C>{std::make_shared<Widget>,Widget{}};

  // std::cerr << o->first.i_ << " " << o->second.i_ << std::endl;


  auto w = Widget{};
  auto ptr = std::make_shared<Widget>();


  // auto p = std::pair{ptr,w};

  // auto o = std::make_optional<C>(ptr,w);
   if (false)
   return std::nullopt;
  return std::optional<C>{std::in_place, ptr, std::move(w)};
}

int main() {
  std::cerr << "--------------rvoWidget\n";
  auto w = rvoWidget();
  std::cerr << "--------------rvoPair\n";
  auto wo = rvoPair();
  std::cerr << "--------------rvoOpt\n";
  auto p = rvoOpt();
  std::cerr << "--------------rvoOptPair\n";
  auto o = rvoOptPair();
}
