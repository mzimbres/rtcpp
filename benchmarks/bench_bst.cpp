#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>

#include <container/bst_node.hpp>
#include <container/bst.hpp>

#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

int main(int argc, char* argv[])
{
  std::size_t size = 6000000;
  if (argc == 2)
    size = to_number<std::size_t>(argv[1]);

  using namespace rtcpp;

  const int a = 1;
  const int b = std::numeric_limits<int>::max();
  //const int b = size;

  std::vector<int> data = make_rand_data(size, a, b);
  std::vector<int> data_copy = data;

  std::vector<char> buffer(data.size() * sizeof (bst<int>::node_type));
  std::vector<char> buffer2(2 * data.size() * sizeof (bst<int>::node_type));

  // The three containers we will benchmark.
  bst<int> t1(allocator<int>(&buffer.front(), buffer.size())); // Uses a vector as buffer.
  bst<int, std::less<int>, std::allocator<int>> t3((std::allocator<int>())); // Uses an allocator (more fragmented)
  std::set<int> t4;
  std::set<int, std::less<int>, allocator<int>> t5(std::less<int>(), allocator<int>(&buffer2.front(), buffer2.size()));

  // Benchmarks.
  {
    std::clog << "Insertion: bst (rtcpp::allocator):      ";
    timer t;
    t1.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Insertion: bst (std::allocator):        ";
    timer t;
    t3.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Insertion: std::set:                    ";
    timer t;
    t4.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Insertion: std::set (rtcpp::allocator): ";
    timer t;
    t5.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Sort with std::sort:                    ";
    timer t;
    std::sort(std::begin(data_copy), std::end(data_copy));
  }
  {
    std::clog << "Lookup: bst (rtcpp::allocator):         ";
    timer t;
    t1.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Lookup: bst (std::allocator):           ";
    timer t;
    t3.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Lookup: std::set:                       ";
    timer t;
    t4.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Lookup: std::set (rtcpp::allocator):    ";
    timer t;
    t5.insert(std::begin(data), std::end(data));
  }
  {
    std::clog << "Deletion: bst (rtcpp::allocator):       ";
    timer t;
    t1.clear();
  }
  {
    std::clog << "Deletion: bst (std::allocator):         ";
    timer t;
    t3.clear();
  }
  {
    std::clog << "Deletion: std::set:                     ";
    timer t;
    t4.clear();
  }
  {
    std::clog << "Deletion: std::set (rtcpp::allocator):  ";
    timer t;
    t5.clear();
  }

  return 0;
}

