#include <cstdio>
#include <array>

// listing 2-9
void listing_2_9() {
  std::array<int, 4> arr{ 1, 2, 3, 4 };
  printf("Listing 2-9: The third element is %d.\n", arr[2]);
  arr[2] = 100;
  printf("Listing 2-9: The third element is %d.\n", arr[2]);
}

#include <cxxabi.h> 
#include <typeinfo>

void listing_2_10() {
  int status{-1};
  auto maximum = 0UL;
  std::array<unsigned long, 5> values{ 10, 50, 20, 40, 0 };

  for(auto item : values) {
    if(item > maximum)
      maximum = item;
  }
  
  const char* maximum_type{typeid(maximum).name()};
  printf("Listing 2-10: The maximum value type reads %s initially\n", maximum_type);
  maximum_type = abi::__cxa_demangle(maximum_type, NULL, NULL, &status); 
  printf("Listing 2-10: The maximum value type is %s, mangled with status: %d.\n", maximum_type, status);
  printf("Listing 2-10: The maximum value is %lu.\n", maximum);
}

void listing_2_11() {
  unsigned long maximum = 0;
  std::array<unsigned long, 6> values{ 10, 50, 20, 40, 60, 0 };
  for(auto value : values) {
    if(value > maximum)
      maximum = value;
  }

  printf("Listing 2-11: The maximum value is %lu.\n", maximum);
}

int main() {
  listing_2_9();
  listing_2_10();
  listing_2_11();
}