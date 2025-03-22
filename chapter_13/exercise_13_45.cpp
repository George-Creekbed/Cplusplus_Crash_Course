#include <chrono>
#include <cstdio>
#include <random>
#include <vector>
#include <map>
#include <unordered_map>

// adapted from exercise_13_1
// non-compile-time recursive calls to 2 functions below is too slow
unsigned long fibonacci(const size_t input) {
  if (input == 0) return 0UL;
  if (input < 3) return 1UL;
  return fibonacci(input - 1) + fibonacci(input - 2);
}

std::vector<unsigned long> createFibonacciSeries(const size_t n) { 
  std::vector<unsigned long> ret{1, 1};
  ret.reserve(n);

  for (size_t i=2; i!=n+1; ++i)
    ret.emplace_back(ret[i-1] + ret[i-2]);
    // ret.emplace_back(fibonacci(i));

  return ret;
} 
//

// it's faster to implement fibonacci logic directly in a loop
// elapsed (with push_back): 4.6971 s (with emplace_back): 4.52805 s
long fib_sum(size_t n) {
  std::vector<unsigned long> my_vec{1, 1};
  my_vec.reserve(n);
  for (size_t i=2; i!=n+1; ++i)
    my_vec.emplace_back(my_vec[i-1] + my_vec[i-2]); 

  //// elapsed (with added function call as below): 5.07529 s
  // std::vector<unsigned long> my_vec(n);
  // my_vec = createFibonacciSeries(n);

  long vec_sum{};
  for (auto item : my_vec)
    vec_sum += item;

  return vec_sum;
}

long random() {
  static std::mt19937_64 mt_engine{102787};
  static std::uniform_int_distribution<long> int_d{1000, 2000};

  return int_d(mt_engine);
}

struct Stopwatch {
  Stopwatch(std::chrono::nanoseconds& result)
    : result{result},
    start{std::chrono::system_clock::now()} {}
  ~Stopwatch() {
    result = std::chrono::system_clock::now() - start;
  }

private:
  std::chrono::nanoseconds& result;
  const std::chrono::time_point<std::chrono::system_clock> start;
};

// elapsed (with emplace): 0.120501 s (with insert): 0.122099 s
long cached_fib_sum(const size_t n) {
  static std::map<long, long> cache; // it's STATIC
  if (cache.find(n) != cache.end())
    return cache[n];
  // below, emplaces new cache value if not there already,
  // returns requested value in any case
  return (*cache.emplace(n, fib_sum(n)).first).second;  // faster than insert
  // return (*cache.insert(std::make_pair(n, fib_sum(n))).first).second;
}

// elapsed: 0.0270106 s
long cached_fib_sum_unordered(const size_t n) {
  static std::unordered_map<long, long> cache(n); // it's STATIC, n buckets
  // cache.reserve(n);  // slows down operations considerably
  if (cache.find(n) != cache.end())
    return cache[n];

  return (*cache.emplace(n, fib_sum(n)).first).second;
}

// elapsed: 0.0199488 s winner
long cached_fib_sum_vector(const size_t n) {
  static std::vector<long> cache{1, 2}; // it's STATIC
  cache.reserve(2000); 

  if (cache[n-1] == 0)
    cache[n-1] = fib_sum(n);
 
  return cache[n-1];
}

int main() {
  size_t samples{1'000'000};
  std::chrono::nanoseconds elapsed;
  {
    Stopwatch stopwatch{elapsed};
    volatile double answer;
    while (samples--) {
      // answer = fib_sum(random());
      // answer = cached_fib_sum(random());
      // answer = cached_fib_sum_unordered(random());
      answer = cached_fib_sum_vector(random());
    }
  }
  printf("Elapsed: %g s.\n", elapsed.count() / 1'000'000'000.);
}