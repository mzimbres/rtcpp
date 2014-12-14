#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <container/bst.hpp>
#include <memory/allocator.hpp>

int main()
{
  rt::bst<int> t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

