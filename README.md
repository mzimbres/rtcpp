RTCPP
============

  The idea of this project is to implement real-time data structures and algorithms
  in C++. Most of what is implemented here was taken from the book "The Art of Computer
  programming Vol. 1 and 3".

Node based
=============

  (This is still unfinished work)
  Some of the problems when using standard node-based containers (with default
  allocators) are:

  1) Dynamic allocations (with system calls) on every insertion.

  2) Heap fragmentation that impact performance, specially for small data types
     like POD (plain old data).

  3) More suitable when the containers must grow on demand. If you do not need
     this feature, you are paying for what you do not need.

  4) Unsuitable for constant time allocations i.e. allocations that do not
     depend on the container size and on heap fragmentation.
  
  To avoid these problems one has to write custom allocators and this is one
  of the motivations for this project. The idea is to use a block of nodes and
  link the nodes together to form an avail stack. That way allocations are
  performed by pushing and poppig fro the stack. However, since the container
  node_type is not exposed, we need a workaround.

Binary Search Trees
===================

Let me list some important facts about my implementation of a Binary Search Tree
(the bst class)

1) It is real-time when used toguether with the allocator I have written. The
   time taken to allocate a node is constant and independent of the heap state.
   But since the tree is unbalanced no logarithmic time can be guaranteed and that
   may be undesirable in real-time applications.

2) On large data sets with uniform distribution, it is extremely unlikely that an
   unbalanced tree will arise. The reader should not exclude an unbalanced bst
   from cosideration just because it does not guarantee logarithmic time, as that
   would also prevent us from using algorithms like quick sort.

3) Its design is the same as std::set.  The are only some missing functions
   currently. It is meant to be C++14 compliant.

To play with the benchmarks use the program bench_bst.

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

