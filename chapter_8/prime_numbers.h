#pragma once

struct PrimeNumberIterator {
  explicit PrimeNumberIterator(const int);
  ~PrimeNumberIterator();

  bool operator!=(const int) const;
  bool sieveOfEratosthenes();
  PrimeNumberIterator& operator++();
  int operator*() const;

  private:
  int i{ 2 }, max{};
  bool* is_prime;
};

struct PrimeNumberRange {
  explicit PrimeNumberRange(const int);

  PrimeNumberIterator begin() const;
  int end() const;

  private:
  const int max;
};