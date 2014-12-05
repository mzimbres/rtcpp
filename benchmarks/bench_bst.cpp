#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>

#include <trees/bst_node.hpp>
#include <trees/bst.hpp>

#include <utility/node_stack.hpp>
#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

template <typename Set, typename InputIt>
void fill_set(Set& set, InputIt begin, InputIt end)
{
  for (InputIt iter = begin; iter != end; ++iter) {
    auto pair = set.insert(*iter);
    if (!pair.second)
       continue;
  }
}

int main()
{
  using namespace rtcpp;

  try {
    const std::size_t size = 6000000;
    const int a = 1;
    const int b = std::numeric_limits<int>::max();
    // Use this limit to make the tree more likely unbalanced.
    //const int b = size;

    std::vector<int> data = make_rand_data(size, a, b);

    typedef bst_node<int> node_type;
    std::vector<char> buffer(data.size() * sizeof (node_type));

    allocator<int> alloc(&buffer[0], buffer.size());

    // The three containers we will benchmark.
    bst<int> t1(std::ref(alloc)); // Uses a vector as buffer.
    bst<int, std::less<int>, std::allocator<int>> t3((std::allocator<int>())); // Uses an allocator (more fragmented)
    std::set<int> t4;

    // Benchmarks.
    {
      std::clog << "Insertion: bst (rtcpp::allocator): ";
      timer t;
      fill_set(t1, std::begin(data), std::end(data));
    }
    {
      std::clog << "Insertion: bst (std::allocator):   ";
      timer t;
      fill_set(t3, std::begin(data), std::end(data));
    }
    {
      std::clog << "Insertion: std::set:               ";
      timer t;
      fill_set(t4, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    bst (vector):           ";
      timer t;
      fill_set(t1, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    bst (std::allocator):   ";
      timer t;
      fill_set(t3, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    std::set:               ";
      timer t;
      fill_set(t4, std::begin(data), std::end(data));
    }
    {
      std::clog << "Deletion: bst (vector):            ";
      timer t;
      t1.clear();
    }
    {
      std::clog << "Deletion: bst (std::allocator):    ";
      timer t;
      t3.clear();
    }
    {
      std::clog << "Deletion: std::set:                ";
      timer t;
      t4.clear();
    }
  } catch (...) {
    return 1;
  }

  return 0;
}

