#pragma once

#include <iterator>

namespace rtcpp {
  template <std::size_t n>
  struct dot_product_impl {
    template <typename RIter1, typename RIter2> // P is a random access iter.
    static typename std::iterator_traits<RIter1>::value_type apply(RIter1 p1, RIter2 p2)
    { return p1[n - 1] * p2[n - 1] + dot_product_impl<n - 1>::apply(p1, p2); }
  };

  template <>
  struct dot_product_impl<1> {
    template <typename RIter1, typename RIter2>
    static typename std::iterator_traits<RIter2>::value_type apply(RIter1 p1, RIter2 p2)
    { return p1[0] * p2[0]; }
  };

  template <std::size_t n, typename RIter1, typename RIter2>
  typename std::iterator_traits<RIter1>::value_type dot_product(RIter1 p1, RIter2 p2)
  { return dot_product_impl<n>::apply(p1, p2); }
}

