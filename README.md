##RTCPP


  The idea of this project is to implement a real-time version of the C++
  Standard Template Library (STL).

  "Real-time" here means a guarantee that the program will execute a particular
  task in at most n steps (or processor cycles). To honor this demand we have
  to get rid of all sources of unpredictable behaviour. Some of them are:

  1. Heap fragmentation.
  2. Use of operator new.
  3. Use of exceptions.

  Before we talk about how I addressed these problems, let us see some examples.

##Real-time version of std::set.

The reader can find these examples in the directory "examples". For benchmarks,
see the program bench_set. So let us see the first example.

```c++
  rt::set<int> t1 = {5, 3, 7, 20, 1, 44, 22, 8}; // equivalent of std:set.

  std::copy( std::begin(t1)
           , std::end(t1)
           , std::ostream_iterator<int>(std::cout, " "));

  std::cout << std::endl;
```

The output of this program is "1 3 5 7 8 20 22 44". The class `rt::set` is the
equivalent of `std::set` but it makes no use of exceptions and expects the
allocator to return a null pointer when allocation fails. That means it is not
safe to use it with `std::allocator`. Since we did not provide an allocator it
is using a default constructed one, that provides storage for 1kb of
storage. To use a different storage size one needs an explicit allocator.

```c++
  std::array<char, 200> buffer = {{}}; // 200 bytes buffer.
  rt::allocator<int> alloc(buffer);
  rt::set<int> t1(alloc);
  rt::set<int> t2(alloc);

  t1 = {5, 3, 7, 20, 1};
  t2 = {44, 22, 8, 44, 33};

  // Print t1 and t2 ...
```
In the code snippet above, we see a 200 bytes buffer being shared by t1 and t2.
The program outputs "1 3 5 7 20 " and "22 44", the reason for which t2 does not
contain the five numbers is that the 200 bytes buffer is not enough storage for
both t1 and t2 (remember the `node_type` must be allocated inside the buffer).

The real-time version of `std::set` has the same interface as std::set itself but
requires more guarantees on its template parameters i.e. the compare function
and the allocator. Below the reader can see some benchmarks

![Insertion time](fig/set_insertion.pdf),
![Deletion time](fig/set_deletion.pdf)


## How to write a real-time STL 

There are two things in the current C++ standard that prevents me from
using STL on real-time systems (point 2 is meant for node-based containers)

1. Standard allocators are required to throw `std::bad_alloc` on allocation
failure and standard containers rely on that feature. Since on real-time
systems exceptions are not allowed this is an undesired behaviour.

2. Container writers are allowed to use Allocator::allocate(n) with n different
from 1. That prevents me from writing real-time allocation, where nodes are
pre-allocated and linked together to form a stack. That way allocating and
deallocating translates into popping and pushing from the stack. This is a way
of achieving truly constant time allocation as opposed to "amortized".  This
way, no matter how fragmented the heap may be or how long the system has been
running, allocation will last the same time.

##Compilation

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

##Credits

Most of what is implemented here was taken from the book
  "The Art of Computer Programming Vol. 1 and 3".

