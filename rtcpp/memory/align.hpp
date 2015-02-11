#pragma once

#include <cstddef>
#include <cstdint>

namespace rt {

template <std::size_t N>
struct is_power_of_two {
  static constexpr bool value = (N > 0) && ((N & (N - 1)) == 0);
};

// returns true if a is aligned with respect to N i.e. the remainder of a
// divided by b is zero. N must be a power of two.
template <std::size_t N>
constexpr bool is_aligned(std::size_t a) noexcept
{ 
  static_assert(is_power_of_two<N>::value, "is_aligned: N must be a power of two.");
  return (a & (N - 1)) == 0;
}

template <std::size_t N>
constexpr std::size_t align_previous(std::size_t a) noexcept
{
  static_assert(is_power_of_two<N>::value, "align_previous: N must be a power of two.");
  return (a & ~(N - 1));
}

template <std::size_t N>
constexpr std::size_t align_next(std::size_t a) noexcept
{
  static_assert(is_power_of_two<N>::value, "align_next: N must be a power of two.");
  return align_previous<N>(a) + N;
}

template <std::size_t N>
void align_if_needed(char*& p, std::size_t& s) noexcept
{
  // If p is not aligned on an N boundary, this unction will align it and
  // update s if alignment took place.
  static_assert(is_power_of_two<N>::value, "align_next: N must be a power of two.");
  // aligns the pointer on a word boundary.
  const std::uintptr_t a = reinterpret_cast<std::uintptr_t>(p);
  const std::uintptr_t c = is_aligned<N>(a) ? a : align_next<N>(a);
  p = reinterpret_cast<char*>(c);
  s -= c - a;
}

}

