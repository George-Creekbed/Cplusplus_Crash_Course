#include <cstdio>
#include "fibonacci.h"

// using FibonacciIterator;
// using FibonacciRange;

int main() {
  for(const auto i : FibonacciRange{ 5000 }) {
    printf("%d ", i);
  }
}