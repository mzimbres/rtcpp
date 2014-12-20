#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>
#include <set>

#include <container/set.hpp>
#include <memory/allocator.hpp>

int main()
{
  typedef std::set<int, std::less<int>, rt::allocator<int>> rt_set_type;

  std::array<char, 300> buffer = {{}};
  rt::allocator<int> alloc(buffer);

  rt_set_type t1(std::less<int>(), alloc);
  rt_set_type t2(std::less<int>(), alloc);

  try {
    t1 = {5, 3, 7, 20, 1};
    t2 = {44, 22, 8, 44, 33};
  } catch (const std::bad_alloc& e) {
    std::cout << "Thre is not enough memory." << std::endl;
  }

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;

  std::copy( std::begin(t2)
           , std::end(t2)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

