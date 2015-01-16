#pragma once

#include <iostream>
#include <iterator>

namespace rt {

template <typename C>
void print(const C& c, const char* s = " ")
{
  typedef typename C::value_type value_type;
  std::copy( std::begin(c)
           , std::end(c)
           , std::ostream_iterator<value_type>(std::cout, s));

  std::cout << std::endl;
}

}

