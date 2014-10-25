RTCPP
============

  The idea of this project is to implement realtime data structures and algorithms
  in C++. Most of what is implemented here was taken from the book "The Art of Computer
  programming Vol. 1 and 3".

Node based
=============

  Node based containers were my main motivation to begin this project. The bad
  thing about standard containers in C++ is the way they handle memory. There are
  some things that make me regret using them:

  1) If you are using the default allocator you are probabily using malloc for
     each new item inserted in your container, malloc in turn makes use of
     system calls to make room for the new inserted item. Imagine yourself
     using new to allocate space for an int as would be the case for
     std::set<int> for example.  To avoid that, one has to use custom
     allocators, however this does no solve all the problems (Even in the
     new C++11 allocator model).

  2) Dynamic allocations on the heap on systems that demand 7 - 24 availability
     are dangerous as you can end up with a very fragmented heap affecting 
     program performace.

  3) Even though one can use memory pools with allocators, they are more suitable
     for situations where the container size must increase on demand and one
     cannot place an upper bound on the maximum container size. In my job I
     often can place an upper bound on the container size. That means I am
     paying for what I do not need.

  This is the problem that allocators cannot handle properly:

  a) It is difficult to write an allocator with constant allocation time.
     With "constant" I mean something that does no depend on the container size
     or on how fragmented the heap may be. I do not mean "amortized" constant
     time. This fact is due mainly to the fact that the node_type used by the
     container is not exposed to the programmer. If they were exposed, once
     could easily achieve contant time by using node links to build an avail
     stack. For example, assuming that node type has an llink, once can do

                    node_pointer q = avail;
                    if (avail)
                      avail = avail->llink;
                    return q;

     Deallocation of a node pointed by p is as simple as

                    if (!p) return;
                    p->llink = avail;
                    avail = p;

     We see that just a couple of instructions are used, which makes allocation
     and deallocation constant time. See the class node_stack for how I handle
     this.

Binary Searce Trees
===================

Let me list some important facts about my implementation of a Binary Search Tree
(the bst class)

1) The class bst is not exception safe if used with std::allocator as it can
   throw std::bad_alloc. However is is guaranteed to be exception safe if used
   with the allocator node_stack. Actually all its member functions are noexcept.

2) If used with node_stack the class is realtime. The time taken to allocate a
   node is constant and independent of the heap state. But since the tree is
   unbalanced no logarithmic time can be guaranteed and that may be undesirable
   in realtime applications.

3) It is very nice to compare its behaviour using std::allocator and
   rtcpp::pool_allocator as the heap fragmentation performance degradation
   becomes evident.

4) Its design is not the same as std::set by its interface is prety similar.
   The are only some missing functions.

5) I think the new C++11 allocator model cannot achieve what I do here. If
   you know how to do it, please let me know.

To play with the benchmarks use the program bench_bst. Example usage:

  If you want to use it with std::allocator you can simply do "bst<int> tmp;"
  But that is unsafe. It is planned to be used with a pool allocator, for
  example:

Compilation
=============

  To compile you will need a C++11 compiler, CMake and Boost. This is the
  command I use on cmake (maybe without all the optimization flags):

  cmake ../../rtcpp/ -DCMAKE_CXX_FLAGS="-Wall -Wextra -Werror -std=c++0x -Ofast -fipa-pta \
  -flto -funsafe-loop-optimizations" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_COMPILER=g++ -DBOOST_ROOT=${BOOST}

  I have experienced some strange problems with std::revers_iterators on gcc. For example:

  - Clang - Release : Tests succeed.
  - Clang - Debug   : Tests succeed.
  - GCC   - Release : Test 3 fails. Unclean compilation
  - GCC   - Debug   : Tests succeed. Unclean compilation

  Compiler versions are:

  GCC 4.8.2
  Clang 3.4

