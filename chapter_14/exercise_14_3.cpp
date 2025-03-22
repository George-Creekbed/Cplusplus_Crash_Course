#include <chrono>
#include <cstdio>
#include <iterator>
#include <vector>
#include <forward_list>

struct Stopwatch {
  Stopwatch(std::chrono::nanoseconds& result)
      : result{ result }
      , start{ std::chrono::system_clock::now() } {}
  ~Stopwatch() {
    result = std::chrono::system_clock::now() - start;
  }

  private:
  std::chrono::nanoseconds& result;
  const std::chrono::time_point<std::chrono::system_clock> start;
};

int main() {
  const size_t n = 3'000'000;
  std::vector<int> my_vec(n, 1);
  std::forward_list<int> my_list(n, 1);
  auto iter_vec = my_vec.begin();
  auto iter_list = my_list.begin();

  // time traversing the vector: slower
  std::chrono::nanoseconds elapsed_vec;
  {
    Stopwatch stopwatch{ elapsed_vec };
    while (iter_vec != my_vec.end())
        std::advance(iter_vec, 1);
  }
  auto time_advance_vec = elapsed_vec.count() / double{ n };
  printf("std::advancing the vector: took %gns per %zu elements in the vector.\n", time_advance_vec, n);

  // time traversing the forward_list: faster than the vector
  std::chrono::nanoseconds elapsed_list;
  {
    Stopwatch stopwatch{ elapsed_list };
    while (iter_list != my_list.end())
        std::advance(iter_list, 1);
  }
  auto time_advance_list = elapsed_list.count() / double{ n };
  printf("std::advancing the forward_list: took %gns per %zu elements in the forward_list.\n", time_advance_list, n);

  // in both cases, runtime-per-single-advance is faster with larger containers 
}