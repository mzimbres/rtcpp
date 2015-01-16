#include <array>
#include <list>
#include <scoped_allocator>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/allocator.hpp>
#include <rtcpp/utility/print.hpp>

int main()
{
  std::array<char, 2000> buffer1 = {{}};
  std::array<char, 2000> buffer2 = {{}};

  typedef rt::allocator<std::list<int, rt::allocator<int>>> inner_alloc_type;
  typedef std::scoped_allocator_adaptor< inner_alloc_type
                                       , rt::allocator<int>> outer_alloc_type;

  inner_alloc_type alloc1(buffer1);
  rt::allocator<int> alloc2(buffer2);

  outer_alloc_type alloc(alloc1, alloc2);

  std::list<std::list<int, rt::allocator<int>>, outer_alloc_type> t1(alloc);

  t1.push_back({{5, 3, 7, 20}, alloc2});
  t1.push_back({{1, 44, 22, 8}, alloc2});

  rt::print(t1.front());
  rt::print(t1.back());
}

