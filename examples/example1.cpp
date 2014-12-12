#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <container/bst.hpp>
#include <memory/allocator.hpp>

int main()
{
  using namespace rtcpp;

  // The buffer from which the set will allocate memory.
  // Memory is on stack.
  std::array<char, 8 * sizeof (bst<int>::node_type)> buffer;

  // The allocator
  allocator<int> alloc(&buffer.front(), buffer.size());

  // Our realtime version of std::set with random data.
  bst<int> t1(alloc);

  t1 = {3, 5, 7, 20, 1, 44, 22, 8};

  // Outputs it
  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

