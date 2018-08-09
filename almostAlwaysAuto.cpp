#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std::chrono_literals;
using namespace std::string_literals;

auto foo() -> std::unique_ptr<int>;

struct Widget {
  std::shared_ptr<int> ptr_;

  auto getPtr() {
    return ptr_;
  }
};

auto foo() -> std::unique_ptr<int> {
  return std::make_unique<int>(5);
}

std::optional<std::unique_ptr<int>> ptr_factory() {
  return std::make_unique<int>(10);
}

auto struct_bind() {
  return std::make_pair(true, std::make_unique<int>(5));
}

auto main() -> int {
  auto i = int{1};
  auto ii = 2;
  auto ili = {3};

  int is{4};
  auto ias = {5};

  std::cout << "Init List? " << std::size(ias) << std::endl;

  auto opt = std::optional<int>{1};
  auto s = "42"s;

  auto x = 42ns;
  std::cout << std::size(s) << std::endl;
  std::cout << x.count() << std::endl;

  auto ptr = foo();
  auto widget = Widget{foo()};

  if (auto exists = ptr_factory; exists) {
    std::cout << "Läuft" << std::endl;
  }

  if (auto const& [ok, ptr] = struct_bind(); ok) {
    std::cout << "struct bindings Läuft" << std::endl;
  }

  auto vec = std::vector{1, 2, 3};
  std::set set_{4, 5, 6};

  auto ulala = 42u;
  auto lust = {1};

  std::cerr << "Init List length: " << std::size(lust) << std::endl;
  std::cerr << "TEST: " << ulala << std::endl;


  auto w = Widget{};
  auto b = std::move(w);

  auto p = std::pair(2, 4.5);
}





