#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <map>
#include <set>
#include <utility>

#include <memory/allocator.hpp>
#include <container/set.hpp>
#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>
#include <utility/timer.hpp>

template <typename Iter>
void heap_frag( rt::set<int>& s1
              , rt::set<int>& s2
              , rt::set<int>& s3
              , std::map<int, int>& s4
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
  std::map<int, int> s8;
  for (Iter iter = begin; iter != begin + n; ++iter) {
    s1.insert(*iter);
    s5.insert(*iter);
    s2.insert(*iter);
    s6.insert(*iter);
    s3.insert(*iter);
    s7.insert(*iter);
    s4.insert(std::make_pair(*iter, *iter));
    s8.insert(std::make_pair(*iter, *iter));
  }
  // s2 and s4 are destructed leaving many holes in the heap.
}

template <typename C, typename Iter>
void print_bench(C& c, Iter begin, std::size_t n)
{
  {
    rt::timer t;
    for (Iter iter = begin; iter != begin + n; ++iter)
      c.insert(std::make_pair(*iter, *iter));
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
    "The benchmarks have the following layout: \n"
    "N (std::map<std::alloc>) (std::map<rt::alloc>): \n"
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
  std::map<int, int> s4;
  if (frag) 
    heap_frag(s1, s2, s3, s4, std::begin(data), data.size());

  for (std::size_t i = 0; i < K; ++i) {
    const std::size_t ss = N + i * S;
    std::cout << ss << " ";
    {
      std::map<int, int> s;
      print_bench(s, std::begin(data), ss);
    }
    {
      std::vector<char> buffer((ss + 2) * 60, 0);
      rt::allocator<std::pair<const int,int>> alloc(buffer);
      std::map<int, int, std::less<int>, rt::allocator<std::pair<const int,int>>> s(std::less<int>(), alloc); // Uses a vector as buffer.
      print_bench(s, std::begin(data), ss);
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  return 0;
}

