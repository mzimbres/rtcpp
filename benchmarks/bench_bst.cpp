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

#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>

struct options {
  bool   help;
  int size;

  options()
  : help(false)
  , size(2000000)
  {}
};

namespace po = boost::program_options;

options parse_options(int argc, char* argv[])
{
  std::string d_file;
  options op;
  po::options_description desc(
  "\nBenchmarks my implementation of an unbalanced binary search tree against std::set\n"
  "and boost::flat_set. Three benchmarks are made, the first one is the time taken to \n"
  "insert the value_type's. The second one to the time taken to look each one of the \n"
  "inserted items and the thrird the time to destruct the tree.");
  desc.add_options()
    ("help,h", "Available options.")
    ("size,i", po::value<int>(&op.size)->default_value(5000000),
    "Size of random data to be inserted in the set. But note that the data is made "
    "unique and the final size can be less than that.")
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
    // Use this limit to make the tree more likely unbalanced.
    //const int b = size;

    std::vector<int> data = make_rand_data(op.size, a, b);

    // We have to know the node_type used by the tree in order to build an avail
    // stack.
    typedef bst<int>::node_type node_type;

    // The pool allocator type.
    typedef node_stack<node_type> allocator_type;

    // The container type.
    typedef bst<int, std::less<int>, allocator_type> set_type;

    std::vector<node_type> buffer(data.size()); // Node buffer
    node_type* const avail = link_stack(std::begin(buffer), std::end(buffer));
    allocator_type pool(avail);

    // The three containers we will benchmark.
    set_type t1(pool);
    bst<int> t2;
    std::set<int> t3;

    // Benchmarks.
    {
      std::clog << "Insertion: bst (pool): ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t1, std::begin(data), std::end(data));
    }
    {
      std::clog << "Insertion: bst:        ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t2, std::begin(data), std::end(data));
    }
    {
      std::clog << "Insertion: std::set:   ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t3, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    bst (pool): ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t1, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    bst:        ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t2, std::begin(data), std::end(data));
    }
    {
      std::clog << "Lookup:    std::set:   ";
      boost::timer::auto_cpu_timer timer;
      fill_set(t3, std::begin(data), std::end(data));
    }
    //{
    //  std::clog << "Lookup:    flat_set:   ";
    //  boost::container::flat_set<int> t4;
    //  fill_set(t4, std::begin(data), std::end(data));
    //  boost::timer::auto_cpu_timer timer;
    //  fill_set(t4, std::begin(data), std::end(data));
    //}
    {
      std::clog << "Deletion: bst (pool):  ";
      boost::timer::auto_cpu_timer timer;
      t1.clear();
    }
    {
      std::clog << "Deletion: bst:         ";
      boost::timer::auto_cpu_timer timer;
      t2.clear();
    }
    {
      std::clog << "Deletion: std::set:    ";
      boost::timer::auto_cpu_timer timer;
      t3.clear();
    }
  } catch (...) {
    return 1;
  }

  return 0;
}

