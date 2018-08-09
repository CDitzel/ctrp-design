#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

class num_iterator {
  size_t i;
 public:
  explicit num_iterator(size_t position)
      : i{position} {}
  size_t operator*() const { return i; }
  num_iterator &operator++() {
    ++i;
    return *this;
  }

  bool operator!=(const num_iterator &other) const {
    return i!=other.i;
  }
};

using cmplx = std::complex<double>;
using csignal = std::vector<cmplx>;

[[nodiscard]]
csignal fourier_transform(csignal const &s, bool back = false) {
  auto const pol = 2.0*M_PI*(back ? -1.0 : 1.0);
  auto const div = back ? 1.0 : static_cast<double>(std::size(s));


  // closure builders in the form of lambdas enable local definitions in scopes
  auto sum_up = [=, &s](size_t j) {
    return [=, &s](cmplx c, size_t k) {
      return c + s[k]*std::polar(1.0, pol*k*j/std::size(s));
    };
  };

  auto to_ft = [=, &s](size_t j) {
    return std::accumulate(num_iterator{0}, num_iterator{std::size(s)},
                           cmplx{}, sum_up(j))/div;
  };

  auto t = csignal(std::size(s));
  std::transform(num_iterator{0}, num_iterator{std::size(s)}, std::begin(t),
                 to_ft);
  return t;
}

// cosine signal generator with mutable lambda
static auto gen_cosine(size_t period_len) {
  return [period_len, n = 0]() mutable {
    return std::cos(n++*2.0*M_PI/period_len);
  };
}

// square signal generator
static auto gen_square_wave(size_t period_len) {
  return [period_len, n = period_len*7/4]() mutable {
    return ((n++*2/period_len)%2)*2 - 1.0;
  };
}

template<typename F>
static csignal signal_from_generator(size_t len, F gen) {
  auto r = csignal(len);
  std::generate(std::begin(r), std::end(r), gen);
  return r;
}

static void print_signal(const csignal &s) {
  auto real_val = [](cmplx c) { return c.real(); };
  transform(begin(s), end(s),
            std::ostream_iterator<double>{std::cout, " "}, real_val);
  std::cout << '\n';
}

int main() {

  auto const sig_len = 100u;

  auto cosine = signal_from_generator(sig_len, gen_cosine(sig_len/2));
  auto square_wave =
      signal_from_generator(sig_len, gen_square_wave(sig_len/2));

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