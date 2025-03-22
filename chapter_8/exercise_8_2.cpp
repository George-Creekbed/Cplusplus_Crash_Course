#include <cstdio>
#include "prime_numbers.h"

int main() {
  for(const auto i : PrimeNumberRange{ 5000 }) 
    printf("%d ", i);
}
