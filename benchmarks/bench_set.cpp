#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>
#include <list>
#include <ext/pool_allocator.h>
#include <ext/bitmap_allocator.h>
#include <ext/mt_allocator.h>

#include <container/bst_node.hpp>
#include <container/set.hpp>
#include <memory/allocator.hpp>
#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

template <typename Iter>
void heap_frag( rt::set<int>& s1
              , rt::set<int>& s2
              , rt::set<int>& s3
              , std::set<int>& s4
              , Iter begin
              , std::size_t n)
{
  s1.clear();
  s2.clear();
  s3.clear();
  s4.clear();
  rt::set<int> s5;
  rt::set<int> s6;
  rt::set<int> s7;
  std::set<int> s8;
  for (Iter iter = begin; iter != begin + n; ++iter) {
    s1.insert(*iter);
    s5.insert(*iter);
    s2.insert(*iter);
    s6.insert(*iter);
    s3.insert(*iter);
    s7.insert(*iter);
    s4.insert(*iter);
    s8.insert(*iter);
  }
  // s2 and s4 are destructed leaving many holes in the heap.
}

template <typename C, typename Iter>
void print_bench(C& c, Iter begin, std::size_t n)
{
  {
    rt::timer t;
    c.insert(begin, begin + n);
  }
}

int main(int argc, char* argv[])
{
  if ((argc != 4) && (argc != 5)) {
    std::cout <<
    "\nUsage: $ ./bench_set N S K F\n"
    "N: The start size.\n"
    "S: The step size.\n"
    "K: How many steps.\n"
    "F: Optional. If provided will not fragment the heap before benchmarks.\n"
    << std::endl;
    std::cout <<
    "The program output has the following layout: \n"
    "(0) (1) (2) (3) (4) (5) (6) (7) (8) (9) (10): \n"
    "Where: \n"
    "(0)  Number of elements.\n"
    "(1)  rt::set<std::alloc>\n"
    "(2)  rt::set<rt::alloc>\n"
    "(3)  rt::set<__gnu_cxx::__pool_alloc>\n"
    "(4)  rt::set<__gnu_cxx::bitmap_alloc>\n"
    "(5)  rt::set<__mt_alloc>\n"
    "(6)  std::set<std::alloc>\n"
    "(7)  std::set<rt::alloc>\n"
    "(8)  std::set<__gnu_cxx::__pool_alloc>\n"
    "(9)  std::set<__gnu_cxx::bitmap_alloc>\n"
    "(10) std::set<__mt_alloc>\n"
    << std::endl;

    return 0;
  }

  const std::size_t N = to_number<std::size_t>(argv[1]);
  const std::size_t S = to_number<std::size_t>(argv[2]);
  const std::size_t K = to_number<std::size_t>(argv[3]);
  bool frag = true;
  if (argc == 5)
    frag = false;

  if (S > N) {
    std::cout << "S must be less than N." << std::endl;
    return 1;
  }

  const int a = 1;
  const int b = std::numeric_limits<int>::max();

  const std::size_t size = N + K * S;
  std::vector<int> data = rt::make_rand_data<int>(size, a, b);

  rt::set<int> s1;
  rt::set<int> s2;
  rt::set<int> s3;
  std::set<int> s4;
  if (frag) 
    heap_frag(s1, s2, s3, s4, std::begin(data), data.size());

  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t ss = N + i * S;
    std::cout << ss << " ";
    { // (1)
      rt::set<int> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (2)
      std::vector<char> buffer((ss + 2) * sizeof (rt::set<int>::node_type), 0);
      rt::allocator<int> alloc(buffer);
      rt::set<int, std::less<int>, rt::allocator<int>> s(alloc); // Uses a vector as buffer.
      print_bench(s, std::begin(data), ss);
    }
    { // (3)
      rt::set<int, std::less<int>, __gnu_cxx::__pool_alloc<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (4)
      rt::set<int, std::less<int>, __gnu_cxx::bitmap_allocator<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (5)
      rt::set<int, std::less<int>, __gnu_cxx::__mt_alloc<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (6)
      std::set<int> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (7)
      std::vector<char> buffer((ss + 2) * (2 * sizeof (rt::set<int>::node_type)), 0);
      rt::allocator<int> alloc(buffer);
      std::set<int, std::less<int>, rt::allocator<int>> s(std::less<int>(), alloc);
      print_bench(s, std::begin(data), ss);
    }
    { // (8)
      std::set<int, std::less<int>, __gnu_cxx::__pool_alloc<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (9)
      std::set<int, std::less<int>, __gnu_cxx::bitmap_allocator<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    { // (10)
      std::set<int, std::less<int>, __gnu_cxx::__mt_alloc<int>> s;
      print_bench(s, std::begin(data), ss);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return 0;
}

