#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <container/bst.hpp>
#include <memory/allocator.hpp>

int main()
{
  std::array<char, 200> buffer = {{}};
  rt::allocator<int> alloc(buffer);
  rt::bst<int> t1(alloc);
  rt::bst<int> t2(alloc);

  t1 = {5, 3, 7, 20, 1};
  t2 = {44, 22, 8, 44, 33};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;

  std::copy( std::begin(t2)
           , std::end(t2)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

