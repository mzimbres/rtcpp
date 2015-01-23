#pragma once

  /*

  Definition of a node for a threaded binary search tree and its fundamental
  operations.

  */

namespace rt {

template <std::size_t I>
struct index_helper;

template <>
struct index_helper<0> {
  static const std::size_t other = 1;
};

template <>
struct index_helper<1> {
  static const std::size_t other = 0;
};

namespace detail {
  static const int rbit = 1;
  static const int lbit = 2;
}

template <typename T>
struct bst_node {
  typedef T value_type;
  bst_node* link[2];
  int tag;
  value_type key;
};

template <std::size_t I>
struct set_link_null;

template <>
struct set_link_null<0> {
  template <typename T>
  static void apply(bst_node<T>* p) noexcept
  { p->tag |= detail::lbit; }
};

template <>
struct set_link_null<1> {
  template <typename T>
  static void apply(bst_node<T>* p) noexcept
  { p->tag |= detail::rbit; }
};

template <std::size_t I>
struct unset_link_null;

template <>
struct unset_link_null<0> {
  template <typename T>
  static void apply(bst_node<T>* p) noexcept
  { p->tag &= ~detail::lbit; }
};

template <>
struct unset_link_null<1> {
  template <typename T>
  static void apply(bst_node<T>* p) noexcept
  { p->tag &= ~detail::rbit; }
};

template <std::size_t I>
struct has_null_link;

template <>
struct has_null_link<0> {
  template <typename T>
  static int apply(const bst_node<T>* p)
  {return p->tag & detail::lbit;}
};

template <>
struct has_null_link<1> {
  template <typename T>
  static int apply(const bst_node<T>* p)
  {return p->tag & detail::rbit;}
};

template <std::size_t I, typename T> 
bst_node<T>* inorder(const bst_node<T>* p) noexcept
{
  // I = 0: predecessor. I = 1: sucessor.
  if (has_null_link<I>::apply(p))
    return p->link[I];

  bst_node<T>* q = p->link[I];
  while (!has_null_link<index_helper<I>::other>::apply(q))
    q = q->link[index_helper<I>::other];

  return q;
}

template <std::size_t I, typename T> 
const bst_node<T>* inorder_parent(const bst_node<T>* p) noexcept
{
  // I = 0: predecessor. I = 1: sucessor.
  if (has_null_link<I>::apply(p))
    return p->link[I];

  const bst_node<T>* pq = p;
  const bst_node<T>* q = p->link[I];
  while (!has_null_link<index_helper<I>::other>::apply(q)) {
    pq = q;
    q = q->link[index_helper<I>::other];
  }

  return pq;
}

template <typename T>
bst_node<T>* preorder_successor(const bst_node<T>* p) noexcept
{
  if (!has_null_link<0>::apply(p))
    return p->link[0];

  if (!has_null_link<1>::apply(p))
    return p->link[1];

  // This is a leaf node.
  bst_node<T>* q = p->link[1];
  while (has_null_link<1>::apply(q))
    q = q->link[1];

  return q->link[1];
}

template <std::size_t I, typename T>
void attach_node(bst_node<T>* p, bst_node<T>* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->link[I] = p->link[I];
  q->tag = has_null_link<index_helper<I>::other>::apply(q) | has_null_link<I>::apply(p);
  p->link[I] = q;
  p->tag = has_null_link<index_helper<I>::other>::apply(p);
  q->link[index_helper<I>::other] = p;
  set_link_null<index_helper<I>::other>::apply(q);

  if (!has_null_link<I>::apply(q)) {
    bst_node<T>* qs = inorder<I>(q);
    qs->link[index_helper<I>::other] = q;
  }
}

template <typename T>
bst_node<T>* erase_node(bst_node<T>* pq, bst_node<T>* q) noexcept
{
  // p is parent of q. We do not handle the case p = q
  // Returns the erased node to be released elsewhere.
  // WARNING: Still unfinished.
  bst_node<T>** linker = &pq->link[0];
  if (pq->link[1] == q)
    linker = &pq->link[1];
  typedef bst_node<T>* node_pointer;
  if (!has_null_link<0>::apply(q) && !has_null_link<1>::apply(q)) {
    node_pointer u = const_cast<node_pointer>(inorder_parent<1>(q));
    node_pointer s = q->link[1];
    if (u != q)
      s = u->link[0];
    node_pointer p = inorder<0>(q);
    s->link[0] = q->link[0];;
    unset_link_null<0>::apply(s);
    p->link[1] = s;
    if (has_null_link<1>::apply(s))
      set_link_null<0>::apply(u);
    else
      u->link[0] = s->link[1];;
    if (u != q) {
      s->link[1] = q->link[1];;
      unset_link_null<1>::apply(s);
    }
    *linker = s;
    return q;
  }

  if (has_null_link<0>::apply(q) && !has_null_link<1>::apply(q)) {
    node_pointer u = const_cast<node_pointer>(inorder_parent<1>(q));
    node_pointer s = q->link[1];
    if (u != q)
      s = u->link[0];
    s->link[0] = q->link[0];;
    if (has_null_link<1>::apply(s))
      set_link_null<0>::apply(u);
    else
      u->link[0] = s->link[1];;
    if (u != q) {
      s->link[1] = q->link[1];;
      unset_link_null<1>::apply(s);
    }
    *linker = s;
    return q;
  }

  if (!has_null_link<0>::apply(q) && has_null_link<1>::apply(q)) {
    node_pointer u = const_cast<node_pointer>(inorder_parent<0>(q));
    node_pointer s = q->link[0];
    if (u != q)
      s = u->link[1];
    s->link[1] = q->link[1];;
    if (has_null_link<0>::apply(s))
      set_link_null<1>::apply(u);
    else
      u->link[1] = s->link[0];;
    if (u != q) {
      s->link[0] = q->link[0];;
      unset_link_null<0>::apply(s);
    }
    *linker = s;
    return q;
  }
  // Both links are null
  if (pq->link[0] == q) {
    set_link_null<0>::apply(pq);
    pq->link[0] = q->link[0];
  } else {
    set_link_null<1>::apply(pq);
    pq->link[1] = q->link[1];
  }

  return q;
}

}

