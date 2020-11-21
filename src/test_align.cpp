#include "rtcpp.hpp"

void test_align()
{

  static_assert(rt::is_power_of_two( 2), "Error:  2 is a power of two.");
  static_assert(rt::is_power_of_two( 4), "Error:  4 is a power of two.");
  static_assert(rt::is_power_of_two( 8), "Error:  8 is a power of two.");
  static_assert(rt::is_power_of_two(16), "Error: 16 is a power of two.");
  static_assert(rt::is_power_of_two(32), "Error: 32 is a power of two.");
  static_assert(rt::is_power_of_two(64), "Error: 64 is a power of two.");

  static_assert(rt::is_aligned(16, 8), "Error: rt::is_aligned(16, 8)");

  static_assert(rt::align_next(13, 8) == 16, "Error.");
  static_assert(rt::align_next( 1, 4) ==  4, "Error.");
  static_assert(rt::align_next(27, 4) == 28, "Error.");
  static_assert(rt::align_next(27, 8) == 32, "Error.");
}

int main()
{
  test_align();
}

