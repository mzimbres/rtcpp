#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>

#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>
#include <boost/container/flat_set.hpp>

#include <trees/bst_node.hpp>
#include <trees/bst.hpp>
#include <trees/node_stack.hpp>
#include <trees/pool_allocator.hpp>

#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>

struct options {
  bool   help;
  int insertion_size;
  int insertion_repeat;
  int lookup_size;
  int lookup_repeat;

  options()
  : help(false)
  , insertion_size(1000000)
  , insertion_repeat(1)
  , lookup_size(1000000)
  , lookup_repeat(1)
  {}
};

namespace po = boost::program_options;

options parse_options(int argc, char* argv[])
{
  std::string d_file;
  options op;
  po::options_description desc( "Benchmarks bst and std::set.");
  desc.add_options()
    ("help,h", "Available options.")
    ("insertion-size,i", po::value<int>(&op.insertion_size)->default_value(1000000), "Size of random data to be inserted in the set.")
    ("insertion-repeat,r", po::value<int>(&op.insertion_repeat)->default_value(1), "How many times insertion should be repeated.")
    ("lookup-size,l", po::value<int>(&op.lookup_size)->default_value(1000000), "Size of random data to be searched in the set.")
    ("lookup-repeat,e", po::value<int>(&op.lookup_repeat)->default_value(1), "How many times search should be repeated.")
  ;

  po::variables_map vm;        
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);    

  if (vm.count("help")) {
    std::cout << desc << "\n";
    op.help = true;
  }

  return op;
}

template <typename Set, typename InputIt>
void fill_set(Set& set, InputIt begin, InputIt end)
{
  for (InputIt iter = begin; iter != end; ++iter) {
    auto pair = set.insert(*iter);
    if (!pair.second)
       continue;
  }
}

int main(int argc, char* argv[])
{
  using namespace rtcpp;

  try {
    options op = parse_options(argc, argv);

    if (op.help) 
      return 0;

    const int a = 1;
    const int b = std::numeric_limits<int>::max();
    //const int b = size;
    std::vector<int> insertion_data = make_rand_data(op.insertion_size, a, b);
    std::vector<int> lookup_data = make_rand_data(op.lookup_size, a, b);

    typedef bst_node<int> node_type;
    typedef node_stack<node_type> pool_type;
    typedef pool_allocator<node_type> allocator_type;
    typedef bst<int, std::less<int>, allocator_type> set_type;

    {
      std::cerr << "Insertion: bst (pool): ";
      node_stack<node_type> insertion_pool(insertion_data.size());
      allocator_type w1(&insertion_pool);
      set_type t1(w1);
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.insertion_repeat; ++i) {
        fill_set(t1, std::begin(insertion_data), std::end(insertion_data));
        t1.clear();
      }
    }

    {
      std::cerr << "Insertion: bst:        ";
      bst<int> t1;
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.insertion_repeat; ++i) {
        fill_set(t1, std::begin(insertion_data), std::end(insertion_data));
        t1.clear();
      }
    }

    {
      std::set<int> t1;
      std::cerr << "Insertion: std::set:   ";
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.insertion_repeat; ++i) {
        fill_set(t1, std::begin(insertion_data), std::end(insertion_data));
        t1.clear();
      }
    }

    {
      std::cerr << "Lookup:    bst (pool): ";
      node_stack<node_type> lookup_pool(lookup_data.size());
      allocator_type w2(&lookup_pool);
      set_type t1(w2);
      fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.lookup_repeat; ++i)
        fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
    }

    {
      std::cerr << "Lookup:    bst:        ";
      bst<int> t1;
      fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.lookup_repeat; ++i)
        fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
    }

    {
      std::cerr << "Lookup:    std::set:   ";
      std::set<int> t1;
      fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.lookup_repeat; ++i)
        fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
    }

    {
      std::cerr << "Lookup:    flat_set:   ";
      boost::container::flat_set<int> t1;
      fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.lookup_repeat; ++i)
        fill_set(t1, std::begin(lookup_data), std::end(lookup_data));
    }
  } catch (...) {
    return 1;
  }

  return 0;
}

