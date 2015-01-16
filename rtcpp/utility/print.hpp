#pragma once

#include <iostream>
#include <iterator>

namespace rt {

template <typename C>
void print(const C& c)
{
  std::copy( std::begin(c)
           , std::end(c)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

}

