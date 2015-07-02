#pragma once

#include <iterator>

#include "dot_product.hpp"

namespace rtcpp {
  template <std::size_t N, typename RandomAccessIter>
  typename std::iterator_traits<RandomAccessIter>::value_type snorm(RandomAccessIter iter)
  { return dot_product<N>(iter, iter); }
}

