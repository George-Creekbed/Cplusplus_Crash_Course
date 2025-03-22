#include <cstdio>
#include <vector>
#include <utility>

unsigned long fibonacci(const size_t input) {
  if (input == 0) return 0UL;
  if (input < 3) return 1UL;
  return fibonacci(input - 1) + fibonacci(input - 2);
}

template<size_t n>
constexpr std::vector<unsigned long> createFibonacciSeries() { 
  std::vector<unsigned long> ret;
  for (size_t i=1; i!=n+1; ++i)
    ret.emplace_back(fibonacci(i));

  return ret;
} 

int main() {
  std::vector<unsigned long> my_vec;
  printf("initial empty vector capacity: %lu\n", my_vec.capacity());
  my_vec.reserve(10);
  printf("vector capacity after reserving 10 elements: %lu\n", my_vec.capacity());

  const size_t fib_limit{20}; 
  //my_vec.assign(createFibonacciSeries<fib_limit>().begin(), createFibonacciSeries<fib_limit>().end());//function called twice
  my_vec = createFibonacciSeries<fib_limit>();
  printf("vector capacity after assigning fibonaccis: %lu\n", my_vec.capacity());

  printf("Print first %zu Fibonacci numbers:\n", fib_limit);
  for (auto item : my_vec)
    printf("%lu ", item);
  printf("\n");
}