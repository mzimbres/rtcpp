#pragma once

#include <cstddef>

namespace rt {

template <std::size_t N>
struct is_power_of_two {
  static const bool value = (N > 0) && ((N & (N - 1)) == 0);
};

// returns true if a is aligned with respect to N i.e. the remainder of a
// divided by b is zero. N must be a power of two.
template <std::size_t N>
bool is_aligned(std::size_t a) noexcept
{ 
  static_assert(is_power_of_two<N>::value, "is_aligned: N must be a power of two.");
  return (a & (N - 1)) == 0;
}

template <std::size_t N>
std::size_t align_previous(std::size_t a) noexcept
{
  static_assert(is_power_of_two<N>::value, "align_previous: N must be a power of two.");
  return (a & ~(N - 1));
}

template <std::size_t N>
std::size_t align_next(std::size_t a) noexcept
{
  static_assert(is_power_of_two<N>::value, "align_next: N must be a power of two.");
  return align_previous<N>(a) + N;
}

}

