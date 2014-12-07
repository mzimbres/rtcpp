#pragma once

#include <cstddef>

namespace rtcpp {

template <typename T>
struct size_of {
  static const std::size_t size = (sizeof (T));
};

constexpr bool is_power_of_two(std::size_t a) {return !(a & (a - 1));}

// returns true if the remainder of a divided by b is zero, i.e. a % b. b
// must be a power of two.
constexpr bool is_aligned(std::size_t a, std::size_t b) {return !(a & (b - 1));}

constexpr std::size_t next_aligned(std::size_t a, std::size_t b) {return (a & ~(b - 1)) + b;}

}
