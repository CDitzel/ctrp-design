#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>


class num_iterator {
    std::uint64_t n_;

public:
  explicit num_iterator(std::uint64_t position) : n_{position} {}
  std::uint64_t operator*() const { return n_; }
  num_iterator &operator++() {
    ++n_;
    return *this;
  }
  bool operator!=(const num_iterator &other) const { return n_ != other.n_; }
};

using cmplx = std::complex<double>;
using csignal = std::vector<cmplx>;

[[nodiscard]] csignal fourier_transform(csignal const &input_signal, bool back = false) {
  auto const pol = 2.0 * M_PI * (back ? -1.0 : 1.0);
  auto const N = back ? 1.0 : static_cast<double>(std::size(input_signal));

  // closure builders in the form of lambdas enable local definitions in scopes
  auto sum_up = [=, &input_signal](std::uint64_t k) {
    return [=, &input_signal](cmplx c, std::uint64_t n) {
      return c + input_signal[n] * std::polar(1.0, pol * k * n / std::size(input_signal));
    };
  };

  // some functional code, none of the code has been executed thus far
  auto to_ft = [=, &input_signal](std::uint64_t k) {
    return std::accumulate(num_iterator{0}, num_iterator{std::size(input_signal)}, cmplx{},
                           sum_up(k)) / N;
  };

  auto output_signal = csignal(std::size(input_signal));
  std::transform(num_iterator{0}, num_iterator{std::size(input_signal)}, std::begin(output_signal),
                 to_ft);
  return output_signal;
}

namespace{
// cosine signal generator with mutable lambda
 auto gen_cosine(std::uint64_t period_length) {
  return [period_length, n = 0]() mutable {
    return std::cos(n++ * 2.0 * M_PI / period_length);
  };
}
//Each instance of the lambda expression represents a function object that modifies its own state on every call.
// square signal generator
auto gen_square_wave(std::uint64_t period_length) {
  return [period_length, n = period_length * 7 / 4]() mutable {
    return ((n++ * 2 / period_length) % 2) * 2 - 1.0;
  };
}

template <typename F>
auto signal_from_generator(std::uint64_t sample_size, F gen) {
  auto input_signal = csignal(sample_size);
  std::generate(std::begin(input_signal), std::end(input_signal), gen);
  return input_signal;
}

 void print_signal(const csignal &s) {
  auto real_val = [](cmplx c) { return c.real(); };
  transform(begin(s), end(s), std::ostream_iterator<double>{std::cout, " "},
            real_val);
  std::cout << '\n';
}

}

int main() {

  auto const sample_size = 100u;
  auto const period_length = sample_size/2;

  auto cosine = signal_from_generator(sample_size, gen_cosine(period_length));
  auto square_wave =
      signal_from_generator(sample_size, gen_square_wave(period_length));

  auto trans_sqw = fourier_transform(square_wave);
  std::fill(std::next(begin(trans_sqw), 10), std::prev(std::end(trans_sqw), 10),
            0);
  auto mid = fourier_transform(trans_sqw, true);
  print_signal(cosine);
  print_signal(fourier_transform(cosine));
#if 0
  print_signal(mid);
  print_signal(trans_sqw);
  print_signal(square_wave);
  print_signal(fourier_transform(square_wave));
#endif
}
