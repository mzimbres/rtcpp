RTCPP
============

  The idea of this project is to implement real-time data structures and
  algorithms in C++. Most of what is implemented here was taken from the book
  "The Art of Computer Programming Vol. 1 and 3".

  "Realtime" here means there is an upper bound on how long the system takes to
  respond or execute a task. This requirement exclude the use of dynamic
  allocations (with system calls) and the use of exceptions since they
  introduce unpredicted behaviour. I expect this project to be useful embedded
  C++ programming.

  Some problems of STL containers for C++ embedded are:

  1) Dynamic allocations (with system calls) on every insertion and 
     use of exceptions.

  2) Heap fragmentation that impact performance and introduce unpredictability.

  4) Unsuitable for constant time allocations i.e. allocations that do not
     depend on the container size and on heap fragmentation.

  Even though custom allocators help addressing some of these problems,
  they cannot address them all.

Realtime version of std::set.
===================

I have made an implementation of std::set that does not use exception and
allow non-throwing allocators. For a benchmark against std::set see the
program bench_bst. An example use can be seem on the code below taken
from the file examples/example1.cpp

```
  rt::set<int> t1 = {5, 3, 7, 20, 1, 44, 22, 8};

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
```

Compilation
=============

  To compile you will need a C++11 compiler and CMake. This is the command I
  use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations -fno-exceptions -fno-rtti" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have experienced some strange problems with std::revers_iterators on gcc. For example:

  - Clang - Release : Tests succeed.
  - Clang - Debug   : Tests succeed.
  - GCC   - Release : Test 3 fails. Unclean compilation
  - GCC   - Debug   : Tests succeed. Unclean compilation

  Compiler versions are:

  GCC 4.8.2
  Clang 3.4

