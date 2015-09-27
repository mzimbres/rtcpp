#include <array>
#include <list>
#include <scoped_allocator>

#include <rtcpp/container/set.hpp>
#include <rtcpp/memory/node_allocator.hpp>
#include <rtcpp/utility/print.hpp>

int main()
{
  typedef rt::node_allocator<int> inner_alloc_type;
  typedef std::list<int, inner_alloc_type> inner_list_type;

  typedef std::scoped_allocator_adaptor< rt::node_allocator<inner_list_type>
                                       , inner_alloc_type> outer_alloc_type;
  typedef std::list<inner_list_type, outer_alloc_type> outer_list_type;

  std::array<char, 2000> buffer1 = {{}};
  std::array<char, 2000> buffer2 = {{}};

  inner_alloc_type alloc1(buffer1);
  rt::node_allocator<inner_list_type> alloc2(buffer2);

  outer_alloc_type alloc(alloc2, alloc1);

  outer_list_type t1(alloc);

  t1.push_back({{5, 3, 7, 20}, alloc1});
  t1.push_back({{1, 44, 22, 8}, alloc1});

  rt::print(t1.front());
  rt::print(t1.back());
}

