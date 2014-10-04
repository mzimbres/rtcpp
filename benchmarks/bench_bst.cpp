#include <iostream>
#include <iterator>
#include <random>
#include <limits>
#include <functional>
#include <algorithm>
#include <set>

#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>

#include <trees/bst.hpp>
#include <trees/node_pool.hpp>

#include <utility/to_number.hpp>
#include <utility/make_rand_data.hpp>

struct options {
  bool   help;
  int size;
  int repeat;

  options()
  : help(false)
  , size(20)
  , repeat(20)
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
    ("size,s", po::value<int>(&op.size)->default_value(20), "Data size.")
    ("repeat,r", po::value<int>(&op.repeat)->default_value(20), "Number of times to repeat.")
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

    const int size = op.size;
    const int a = 1;
    const int b = std::numeric_limits<int>::max();
    //const int b = size;
    std::vector<int> data = make_rand_data(size, a, b);

    node_pool<int> pool(data.size());
    {
      std::cerr << "Time to fill a bst:       ";
      bst<int> t1(pool);
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.repeat; ++i) {
        fill_set(t1, std::begin(data), std::end(data));
        t1.clear();
      }
    }

    {
      std::set<int> t1;
      std::cerr << "Time to fill an std::set: ";
      boost::timer::auto_cpu_timer timer;
      for (int i = 0; i < op.repeat; ++i) {
        fill_set(t1, std::begin(data), std::end(data));
        t1.clear();
      }
    }
  } catch (...) {
    return 1;
  }

  return 0;
}

