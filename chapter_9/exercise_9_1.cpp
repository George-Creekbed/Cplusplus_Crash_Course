#include <cstdio>

template<typename T> struct CountOver200 {
  size_t operator()(const T* input, size_t length) const {
    size_t counter{};

    for (auto i = 0; i < length; ++i)
      input[i] > 200 ? ++counter : 0;

    return counter;
  }
};

template<typename Fn, typename In, typename Out> 
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
  Out ret{initial};
  for (auto i = 0; i < length; ++i) {
    ret = function(ret, input[i]);
  }

  return ret;
}

int main() {
  int data[] {100, 200, 300, 400, 500};
  size_t data_len = 5;
  // auto sum = fold();
  auto sum = [](auto x, auto y) {return x + y;};
  int result{};
  for (auto item : data)
    result = sum(result, item);
  printf("Sum: %d\n", result);

  auto maximum = [](auto x, auto y) {return x >= y ? x : y;};
  result = data[0];
  for (auto item : data)
    result = maximum(result, item);
  printf("Maximum: %d\n", result);

  auto minimum = [](auto x, auto y) {return x >= y ? y : x;};
  result = data[0];
  for (auto item : data)
    result = minimum(result, item);
  printf("Minimum: %d\n", result);

  CountOver200<int> counter{};
  result = counter(data, data_len);
  printf("Number of elements > 200: %d\n", result);

  printf("Now we try using functional wrapping \"fold\":\n");
  size_t result_functional = fold(sum, data, data_len, 0);
  printf("Result of sum: %zu\n", result_functional);
  result_functional = fold(maximum, data, data_len, data[0]);
  printf("Result of maximum: %zu\n", result_functional);
  result_functional = fold(minimum, data, data_len, data[0]);
  printf("Result of minimum: %zu\n", result_functional);
  auto newCountOver200 = [](int& ret, const int input) {
    input > 200 ? ++ret : 0; 
    return ret;
  };
  result_functional = fold(newCountOver200, data, data_len, 0);
  printf("Result of number of elements > 200: %zu\n", result_functional);
}