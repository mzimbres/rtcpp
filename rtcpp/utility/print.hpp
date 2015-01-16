#pragma once

#include <iostream>
#include <iterator>

namespace rt {

template <typename C>
void print(const C& c)
{
  typedef typename C::value_type value_type;
  std::copy( std::begin(c)
           , std::end(c)
           , std::ostream_iterator<value_type>(std::cout, " "));

  std::cout << std::endl;
}

}

