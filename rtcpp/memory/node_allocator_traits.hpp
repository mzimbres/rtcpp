#pragma once

#include "node_allocator.hpp"

/*
  Adds specialization of allocator_traits for
  rt::node_allocator.
*/

namespace std
{
  template <typename T>
  struct allocator_traits<rt::node_allocator<T>> {
    using allocator_type = typename rt::node_allocator<T>;
    using size_type = typename allocator_type::size_type;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;
    template<typename U>
    using rebind_alloc =
      typename allocator_type::template rebind<U>::other;
    static allocator_type
      select_on_container_copy_construction(const allocator_type& a)
      {return a;}
    static pointer allocate(allocator_type& a, size_type n)
    {return a.allocate(n);}
    static void deallocate( allocator_type& a
                          , pointer p
                          , size_type n) {a.deallocate(p, n);}
    template<class U>
    static void destroy(allocator_type& a, U* p) {a.destroy(p);}
    template<class U, class... Args >
    static void construct(allocator_type& a, U* p, Args&&... args)
    {a.construct(p, std::forward<Args>(args)...);}
  };
}

