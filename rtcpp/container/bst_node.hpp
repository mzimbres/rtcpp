#pragma once

  /*

  Definition of a node for a threaded binary search tree and its fundamental
  operations.

  */

namespace rt {

template <typename T>
struct bst_node {
  typedef T value_type;
  bst_node* link[2];
  int tag;
  value_type key;
};

namespace detail {
static const int rbit = 1;
static const int lbit = 2;
}

template <typename T>
void set_llink_null(bst_node<T>* p) noexcept
{
  p->tag |= detail::lbit;
}

template <typename T>
void set_rlink_null(bst_node<T>* p) noexcept
{
  p->tag |= detail::rbit;
}

template <std::size_t I>
struct has_null_link;

template <>
struct has_null_link<0> {
  template <typename T>
  static int apply(const bst_node<T>* p) {return p->tag & detail::lbit;}
};

template <>
struct has_null_link<1> {
  template <typename T>
  static int apply(const bst_node<T>* p) {return p->tag & detail::rbit;}
};

template <typename T>
bst_node<T>* inorder_successor(const bst_node<T>* p) noexcept
{
  if (has_null_link<1>::apply(p))
    return p->link[1];

  bst_node<T>* q = p->link[1];
  while (!has_null_link<0>::apply(q))
    q = q->link[0];

  return q;
}

template <typename T>
bst_node<T>* inorder_predecessor(const bst_node<T>* p) noexcept
{
  if (has_null_link<0>::apply(p))
    return p->link[0];

  bst_node<T>* q = p->link[0];
  while (!has_null_link<1>::apply(q))
    q = q->link[1];

  return q;
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

template <typename T>
void attach_node_left(bst_node<T>* p, bst_node<T>* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->link[0] = p->link[0];
  q->tag = has_null_link<1>::apply(q) | has_null_link<0>::apply(p);
  p->link[0] = q;
  p->tag = has_null_link<1>::apply(p);
  q->link[1] = p;
  set_rlink_null(q);

  if (!has_null_link<0>::apply(q)) {
    bst_node<T>* qs = inorder_predecessor(q);
    qs->link[1] = q;
  }
}

template <typename T>
void attach_node_right(bst_node<T>* p, bst_node<T>* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->link[1] = p->link[1];
  q->tag = has_null_link<0>::apply(q) | has_null_link<1>::apply(p);
  p->link[1] = q;
  p->tag = has_null_link<0>::apply(p);
  q->link[0] = p;
  set_llink_null(q);

  if (!has_null_link<1>::apply(q)) {
    bst_node<T>* qs = inorder_successor(q);
    qs->link[0] = q;
  }
}

}

