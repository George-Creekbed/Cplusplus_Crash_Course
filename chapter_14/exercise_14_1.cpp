#define CATCH_CONFIG_MAIN
#include <iterator>
#include <catch2/catch.hpp>

TEST_CASE("prev returns iterators at given offsets") {
  std::vector<unsigned char> mission{ 0x9e, 0xc4, 0xc1, 0x29, 0x49, 0xa4, 0xf3, 0x14,
                                      0x74, 0xf2, 0x99, 0x05, 0x8c, 0xe2, 0xb2, 0x2a };
  auto itr1 = mission.begin();
  std::advance(itr1, 4);
  REQUIRE(*itr1 == 0x49);

  auto itr2 = std::prev(itr1);
  REQUIRE(*itr2 == 0x29);

  auto itr3 = std::prev(itr1, 3);
  REQUIRE(*itr3 == 0xc4);

  REQUIRE(*itr1 == 0x49);
}