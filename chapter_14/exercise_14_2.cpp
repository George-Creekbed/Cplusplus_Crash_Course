#define CATCH_CONFIG_MAIN
#include <iterator>
#include <array>
#include <type_traits>
#include <concepts>
#include <utility>
#include <catch2/catch.hpp>

template<typename T> concept Summable = 
    std::is_default_constructible<T>::value
    &&
    requires (T a, T b) {
        {*a += *b} -> int; //std::convertible_to<bool>;
    };

template<Summable T> int sumRange(T begin, T end) {
    int sum{};
    auto my_iter{begin};

    while (my_iter != end) {
        sum += *my_iter;
        my_iter = std::next(my_iter);
    }
    // while (begin != end) { sum += *begin; std::advance(begin, 1); } // works as well

    return sum;
}

TEST_CASE("std::forward_list begin and end provide forward iterators") {
    std::array<int, 5> easy_as{ 1, 2, 3, 4, 5 };

    int sum_easy_as = sumRange(easy_as.begin(), easy_as.end());

    printf("Sum of range: %d\n", sum_easy_as);
    REQUIRE(sum_easy_as == 15);
}