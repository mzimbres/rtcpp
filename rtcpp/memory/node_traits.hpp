#pragma once

#include <array>
#include <vector>
#include <utility>
#include <memory>
#include <exception>
#include <type_traits>

#include "node_stack.hpp"

namespace rt {

template<class T, class R = void>  
struct enable_if_type { using type = R; };

template<class T, class Enable = void>
struct has_pointer : std::false_type
{
  using pointer = Enable;
};

template<class T>
struct has_pointer< T, typename enable_if_type<typename T::pointer>::type> : std::true_type {
  using pointer = typename T::pointer;
};

template<class T, class Enable = void>
struct has_value_type : std::false_type
{
  using value_type = Enable;
};

template<class T>
struct has_value_type< T, typename enable_if_type<typename T::value_type>::type> : std::true_type {
  using value_type = typename T::value_type;
};

template <typename T>
struct is_node_type {
  static const bool value = has_value_type<T>::value && has_pointer<T>::value;
};

// Does not require node pointer to be the same.
template <typename N1, typename N2, bool IsNode = is_node_type<N2>::value && is_node_type<N1>::value>
struct is_same_node_type {
  static const bool has_same_value_type = std::is_same< typename N1::value_type
                                                      , typename N2::value_type>::value;
  using value_type = typename N1::value_type;
  using same_node_type1 = typename N1::template rebind<value_type, void*>::other;
  using same_node_type2 = typename N2::template rebind<value_type, void*>::other;
  static const bool value = has_same_value_type && std::is_same<same_node_type1, same_node_type2>::value;
};

template <typename N1, typename N2>
struct is_same_node_type<N1, N2, false> {
  static const bool value = false;
};

}

