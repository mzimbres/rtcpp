#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>

#include <container/bst_node.hpp>
#include <container/set.hpp>

#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

int main(int argc, char* argv[])
{
  std::size_t size = 600000;
  if (argc == 2)
    size = to_number<std::size_t>(argv[1]);

  using namespace rt;

  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  //const int b = size;

  std::vector<int> data = make_rand_data(size, a, b);
  std::vector<int> data_copy = data;

  std::vector<char> buffer((data.size() + 2) * sizeof (set<int>::node_type));

  // The three containers we will benchmark.
  set<int> t1(allocator<int>(&buffer.front(), buffer.size())); // Uses a vector as buffer.
  set<int, std::less<int>, std::allocator<int>> t3((std::allocator<int>())); // Uses an allocator (more fragmented)
  std::set<int> t4;

  // Benchmarks.
  {
    std::clog << "Insertion: rt::set<rt::alloc>   : ";
    timer t;
    t1.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Insertion: rt::set<std::alloc>  : ";
    timer t;
    t3.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Insertion: std::set<std::alloc> : ";
    timer t;
    t4.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Sort with std::sort             : ";
    timer t;
    std::sort(std::begin(data_copy), std::end(data_copy));
  }
  {
    std::clog << "Lookup: rt::set<rt::alloc>      : ";
    timer t;
    t1.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Lookup: rt::set<std::alloc>     : ";
    timer t;
    t3.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Lookup: std::set<std::alloc>    : ";
    timer t;
    t4.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Deletion: rt::set<rt::alloc>    : ";
    timer t;
    t1.clear();
  }
  {
    std::clog << "Deletion: rt::set<std::alloc>   : ";
    timer t;
    t3.clear();
  }
  {
    std::clog << "Deletion: std::set<std::alloc>  : ";
    timer t;
    t4.clear();
  }

  return 0;
}

