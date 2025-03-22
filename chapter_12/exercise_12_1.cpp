#define CATCH_CONFIG_MAIN
#include <cstdio>
#include <stdexcept>
#include <optional>
#include "./Catch2/catch.hpp"

template <typename To, typename From>
std::optional<To> narrow_cast(From input_value) {
  std::optional<To> converted = static_cast<To>(input_value);
  const auto backwards = static_cast<From>(converted.value());
  if(input_value != backwards)
    converted.reset();

  return converted;
}

TEST_CASE("narrow_cast function returning a std::optional") {
  SECTION("test non-narrowing conversion") {
    int perfect{ 496 };
    const auto perfect_short = narrow_cast<short>(perfect);
    REQUIRE(perfect_short.has_value());
  // Above test analogous to:
  // if (perfect_short.has_value())
  //   printf("perfect_short: %d\n", perfect_short.value());
  // else
  //   printf("Narrow cast of perfect_short.\n");
  }

  SECTION("test narrowing conversion") {
    int cyclic{ 142857 };
    const auto cyclic_short = narrow_cast<short>(cyclic);
    REQUIRE_FALSE(cyclic_short.has_value());
    // Above test analogous to:
    // if (cyclic_short.has_value())
    //   printf("cyclic_short: %d\n", *cyclic_short);
    // else
    //   printf("Narrow cast of cyclic_short.\n");
  }
}

#include <./boost/numeric/conversion/cast.hpp>

TEST_CASE("narrow checking with boost::numeric_cast") {
  SECTION("non-narrowing case") {
    int perfect{ 496 };
    REQUIRE(boost::numeric_cast<short>(perfect) == perfect);
  }

  SECTION("narrowing case") {
    int cyclic{ 142857 };
    REQUIRE_THROWS_AS(boost::numeric_cast<short>(cyclic), boost::numeric::positive_overflow);
  }
}
