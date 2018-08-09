// To implement the map function you should:
// Use std::transform on containers that support iterating and assignment to the
// elements, such as std::vector or std::list:
template <typename F, typename R> R mapf(F &&f, R r) {
  std::transform(std::begin(r), std::end(r), std::begin(r), std::forward<F>(f));
  return r;
}
// Use other means such as explicit iteration and insertion for containers that
// do not support assignment to the elements, such as std::map:
template <typename F, typename T, typename U>
std::map<T, U> mapf(F &&f, std::map<T, U> const &m) {
  std::map<T, U> r;
  for (auto const kvp : m)
    r.insert(f(kvp));
  return r;
}

template <typename F, typename T> std::queue<T> mapf(F &&f, std::queue<T> q) {
  std::queue<T> r;
  while (!q.empty()) {
    r.push(f(q.front()));
    q.pop();
  }
  return r;
}
// To implement the fold function you should:
// Use std::accumulate() on containers that support iterating:
template <typename F, typename R, typename T>
constexpr T foldl(F &&f, R &&r, T i) {
  return std::accumulate(std::begin(r), std::end(r), std::move(i),
                         std::forward<F>(f));
}

template <typename F, typename R, typename T>
constexpr T foldr(F &&f, R &&r, T i) {
  return std::accumulate(std::rbegin(r), std::rend(r), std::move(i),
                         std::forward<F>(f));
}
// Use other means to explicitly process containers that do not support
// iterating, such as std::queue:
template <typename F, typename T>
constexpr T foldl(F &&f, std::queue<T> q, T i) {
  while (!q.empty()) {
    i = f(i, q.front());
    q.pop();
  }
  return i;
}

int main() {
  // Retain absolute values from a vector. In this example, the vector contains
  // both negative and positive values. After applying the mapping, the result
  // is a new vector with only positive values.
  auto vnums = std::vector<int>{0, 2, -3, 5, -1, 6, 8, -4, 9};
  auto r = funclib::mapf([](int const i) { return std::abs(i); }, vnums);
  // r = {0, 2, 3, 5, 1, 6, 8, 4, 9}
  // Square the numerical values of a list. In this example, the list contains
  // integral values. After applying the mapping, the result is a list
  // containing the squares of the initial values.
  auto lnums = std::list<int>{1, 2, 3, 4, 5};
  auto l = funclib::mapf([](int const i) { return i * i; }, lnums);
  // l = {1, 4, 9, 16, 25}
  // Rounded amounts of floating point. For this example, we need to use
  // std::round(); however, this has overloads for all floating point types,
  // which makes it impossible for the compiler to pick the right one. As a
  // result, we either have to write a lambda that takes an argument of a
  // specific floating point type and returns the value of std::round() applied
  // to that value or create a function object template that wraps std::round()
  // and enables its call operator only for floating point types. This technique
  // is used in the following example:
  template <class T = double> struct fround {
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    operator()(const T &value) const {
      return std::round(value);
    }
  };

  auto amounts = std::array<double, 5>{10.42, 2.50, 100.0, 23.75, 12.99};
  auto a = funclib::mapf(fround<>(), amounts);
  // a = {10.0, 3.0, 100.0, 24.0, 13.0}
  // Uppercase the string keys of a map of words (where the key is the word and
  // the value is the number of appearances in the text). Note that creating an
  // uppercase copy of a string is itself a mapping operation. Therefore, in
  // this example, we use mapf to apply toupper() to the elements of the string
  // representing the key in order to produce an uppercase copy:
  auto words = std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}};
  auto m = funclib::mapf(
      [](std::pair<std::string, int> const kvp) {
        return std::make_pair(funclib::mapf(toupper, kvp.first), kvp.second);
      },
      words);
  // m = {{"ONE", 1}, {"TWO", 2}, {"THREE", 3}}
  Normalize values from a queue of priorities-- initially,
      the values are from 1 to 100,
      but we want to normalize them into two values,
      1 = high and 2 = normal.All initial priorities that have a value up
                           to 30 become a high priority,
      the others get a normal priority : auto priorities = std::queue<int>();
  priorities.push(10);
  priorities.push(20);
  priorities.push(30);
  priorities.push(40);
  priorities.push(50);
  auto p =
      funclib::mapf([](int const i) { return i > 30 ? 2 : 1; }, priorities);
  // p = {1, 1, 1, 2, 2}
  // To implement fold, we actually have to consider the two possible types of
  // folding, that is, from left to right and from right to left. Therefore, we
  // have provided two functions called foldl (for left folding) and foldr (for
  // right folding). The implementations shown in the previous section are very
  // similar--they both take a function, a range, and an initial value and call
  // std::algorithm() to fold the values of the range into a single value.
  // However, foldl uses direct iterators, whereas foldr uses reverse iterators
  // to traverse and process the range. The second overload is a specialization
  // for type std::queue, which does not have iterators. Based on these
  // implementations for folding, we can do the following examples:
  Adding the values of a vector of integers.In this case,
      both left and right folding will produce the same result.In the following
          examples,
      we pass either a lambda that takes a sum and a
          number // and returns a new sum or the function object std::plus<>
                 // from the standard library that applies operator+ to two
                 // operands of the same type (basically similar to the closure
                 // of the lambda):
      auto vnums = std::vector<int>{0, 2, -3, 5, -1, 6, 8, -4, 9};

  auto s1 = funclib::foldl([](const int s, const int n) { return s + n; },
                           vnums, 0); // s1 = 22

  auto s2 = funclib::foldl(std::plus<>(), vnums, 0); // s2 = 22

  auto s3 = funclib::foldr([](const int s, const int n) { return s + n; },
                           vnums, 0); // s3 = 22

  auto s4 = funclib::foldr(std::plus<>(), vnums, 0); // s4 = 22
  // Concatenating strings from a vector into a single string:
  auto texts = std::vector<std::string>{"hello"s, " "s, "world"s, "!"s};

  auto txt1 = funclib::foldl(
      [](std::string const &s, std::string const &n) { return s + n; }, texts,
      ""s); // txt1 = "hello world!"

  auto txt2 = funclib::foldr(
      [](std::string const &s, std::string const &n) { return s + n; }, texts,
      ""s); // txt2 = "!world hello"
            // Concatenating an array of characters into a string:
  char chars[] = {'c', 'i', 'v', 'i', 'c'};

  auto str1 = funclib::foldl(std::plus<>(), chars, ""s);
  // str1 = "civic"

  auto str2 = funclib::foldr(std::plus<>(), chars, ""s);
  // str2 = "civic"
  // Counting the number of words from a text based on their already computed
  // appearances available in a map<string, int>:
  auto words = std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}};

  auto count = funclib::foldl(
      [](int const s, std::pair<std::string, int> const kvp) {
        return s + kvp.second;
      },
      words, 0); // count = 6

  // These functions can be pipelined, that is, they can call one function with
  // the result of another. The following example maps a range of integers into
  // a range of positive integers by applying the std::abs() function to its
  // elements. The result is then mapped into another range of squares. These
  // are then summed together by applying a left fold on the range:
  auto vnums = std::vector<int>{0, 2, -3, 5, -1, 6, 8, -4, 9};

  auto s = funclib::foldl(
      std::plus<>(),
      funclib::mapf(
          [](int const i) { return i * i; },
          funclib::mapf([](int const i) { return std::abs(i); }, vnums)),
      0); // s = 236
  // As an exercise, we could implement the fold function as a variadic function
  // template, in the manner seen in a previous recipe. The function that
  // performs the actual folding is provided as an argument:
  template <typename F, typename T1, typename T2>
  auto foldl(F && f, T1 arg1, T2 arg2) {
    return f(arg1, arg2);
  }

  template <typename F, typename T, typename... Ts>
  auto foldl(F && f, T head, Ts... rest) {
    return f(head, foldl(std::forward<F>(f), rest...));
  }
  // When we compare this with the add() function template that we wrote in the
  // recipe Writing a function template with a variable number of arguments, we
  // can notice several differences: The first argument is a function, which is
  // perfectly forwarded when calling foldl recursively. The end case is a
  // function that requires two arguments because the function we use for
  // folding is a binary one (taking two arguments). The return type of the two
  // functions we wrote is declared as auto because it must match the return
  // type of the supplied binary function f that is not known until we call
  // foldl:
  auto s1 = foldl(std::plus<>(), 1, 2, 3, 4, 5);
  // s1 = 15
  auto s2 = foldl(std::plus<>(), "hello"s, ' ', "world"s, '!');
  // s2 = "hello world!"
  auto s3 = foldl(std::plus<>(), 1); // error, too few arguments
}
