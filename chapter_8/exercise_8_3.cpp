#include <cstdio>

struct FibonacciIterator {
  bool operator!=(int x) const {
    return x >= current;
  }

  FibonacciIterator& operator++() {
    const auto tmp = current;
    current += last;
    last = tmp;
    return *this;
  }

  int operator*() const {
    return current;
  }

  private:
  int current{ 1 }, last{ 1 };
};


struct FibonacciRange {
  explicit FibonacciRange(int max)
      : max{ max } {}
  FibonacciIterator begin() const {
    return FibonacciIterator{};
  }
  int end() const {
    return max;
  }

  private:
  const int max;
};


struct PrimeNumberIterator {
  explicit PrimeNumberIterator(const int max)
    : max{max}, is_prime{new bool[max+1]} 
  {
      for (auto i=0; i!=max+1; ++i)
        is_prime[i] = true;
  }
  
  ~PrimeNumberIterator() {
    delete[] is_prime; 
  }

  bool operator!=(int x) const {
    return x >= i;
  }
  
  bool sieveOfEratosthenes() {
    if (is_prime[i]) {
      for (auto j=i*i; j<=max; j+=i) {
        is_prime[j] = false;
      }

      return true;
    }

    return false;
  }

  PrimeNumberIterator& operator++() {
    if (i <= max) {
      sieveOfEratosthenes();
      ++i;
      while (!sieveOfEratosthenes())
        ++i;
    }

    return *this;
  }

  int operator*() const {
    return i;
  }

  private:
  int i{ 2 }, max{};
  bool* is_prime;
};


struct PrimeNumberRange {
  explicit PrimeNumberRange(int max)
    : max{ max } {}

  PrimeNumberIterator begin() const {
    return PrimeNumberIterator{max};
  }

  int end() const {
    return max;
  }

  private:
  const int max;
};


int main() {
  int limit{5000};
  printf("Find Fibonacci numbers up to %u:\n", limit);
  for(const auto i : FibonacciRange{limit}) {
    printf("%d ", i);
  }

  printf("\nFind prime numbers up to %u:\n", limit);
  for(const auto i : PrimeNumberRange{limit}) {
    printf("%d ", i);
  }

  printf("\n");
}