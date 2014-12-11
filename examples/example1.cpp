#include <iostream>
#include <iomanip>
#include <array>
#include <iterator>

#include <container/bst.hpp>
#include <memory/allocator.hpp>

int main()
{
  using namespace rtcpp;

  // Data to be inserted in the set.
  std::array<int, 8> data = {{3, 5, 7, 20, 1, 44, 22, 8}};

  // The buffer from which the set will allocate memory.
  // Memory is on stack.
  std::array<char, 8 * sizeof (bst<int>::node_type)> buffer;

  // The allocator
  allocator<int> alloc(&buffer.front(), buffer.size());

  // Our realtime version of std::set.
  bst<int> t1(alloc);

  // Inserts the data
  t1.insert(std::begin(data), std::end(data));

  // Outputs it
  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
}

