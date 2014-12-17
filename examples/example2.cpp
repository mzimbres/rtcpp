#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <container/set.hpp>
#include <memory/allocator.hpp>

int main()
{
  std::array<char, 2000> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  rt::set<int> t1(alloc);

  t1 = {5, 3, 7, 20, 1, 44, 22, 8, 44, 33, 9, 2, 99, 77};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

