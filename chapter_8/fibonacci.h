#ifndef FIBONACCI_H  // next, use # pragma once
#define FIBONACCI_H

struct FibonacciIterator {
  bool operator!=(int) const;
  FibonacciIterator& operator++();
  int operator*() const;

  private:
  int current{ 1 }, last{ 1 };
};

struct FibonacciRange {
  explicit FibonacciRange(int);
  FibonacciIterator begin() const;
  int end() const;

  private:
  const int max;
};

#endif //FIBONACCI_H