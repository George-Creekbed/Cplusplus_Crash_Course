#include <cstdio>
#include "prime_numbers.h"

PrimeNumberIterator::PrimeNumberIterator(const int max)
  : max{max}, is_prime{new bool[max+1]} 
{
    for (auto i=0; i!=max+1; ++i)
      is_prime[i] = true;
}

PrimeNumberIterator::~PrimeNumberIterator() {
  delete[] is_prime; 
  printf("\nPrimeNumberIterator deleted\n");
}

bool PrimeNumberIterator::operator!=(const int x) const {
  return x >= i;
}

bool PrimeNumberIterator::sieveOfEratosthenes() {
  if (is_prime[i]) {
    for (auto j=i*i; j<=max; j+=i) {
      is_prime[j] = false;
    }

    return true;
  }

  return false;
}

PrimeNumberIterator& PrimeNumberIterator::operator++() {
  if (i <= max) {
    sieveOfEratosthenes();
    ++i;
    while (!sieveOfEratosthenes())
      ++i;
  }

  return *this;
}

int PrimeNumberIterator::operator*() const {
  return i;
}


PrimeNumberRange::PrimeNumberRange(const int max)
  : max{ max } {}

PrimeNumberIterator PrimeNumberRange::begin() const {
  return PrimeNumberIterator{max};
}

int PrimeNumberRange::end() const {
  return max;
}