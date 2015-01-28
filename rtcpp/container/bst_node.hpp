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
  const std::size_t O = index_helper<I>::other;
  if (has_null_link<I>::apply(p))
    return p->link[I];

  bst_node<T>* q = p->link[I];
  while (!has_null_link<O>::apply(q))
    q = q->link[O];

  return q;
}

template <std::size_t I, typename T> 
const bst_node<T>* inorder_parent(const bst_node<T>* p) noexcept
{
  // I = 0: predecessor. I = 1: sucessor.
  const std::size_t O = index_helper<I>::other;
  if (has_null_link<I>::apply(p))
    return p->link[I];

  const bst_node<T>* pq = p;
  const bst_node<T>* q = p->link[I];
  while (!has_null_link<O>::apply(q)) {
    pq = q;
    q = q->link[O];
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
  const std::size_t O = index_helper<I>::other;
  q->link[I] = p->link[I];
  q->tag = has_null_link<O>::apply(q) | has_null_link<I>::apply(p);
  p->link[I] = q;
  p->tag = has_null_link<O>::apply(p);
  q->link[O] = p;
  set_link_null<O>::apply(q);

  if (!has_null_link<I>::apply(q)) {
    bst_node<T>* qs = inorder<I>(q);
    qs->link[O] = q;
  }
}

template <std::size_t I, typename T>
bst_node<T>* erase_node_lr_non_null(bst_node<T>** linker, bst_node<T>* q) noexcept
{
  // I = 0: The inorder predecessor replaces the erased node.
  // I = 1: The inorder sucessor replaces the erased node.
  const std::size_t O = index_helper<I>::other;
  typedef bst_node<T>* node_pointer;
  node_pointer u = const_cast<node_pointer>(inorder_parent<I>(q));
  node_pointer s = q->link[I];
  if (u != q)
    s = u->link[O];
  node_pointer p = inorder<O>(q);
  s->link[O] = q->link[O];;
  unset_link_null<O>::apply(s);
  p->link[I] = s;
  if (has_null_link<I>::apply(s))
    set_link_null<O>::apply(u);
  else
    u->link[O] = s->link[I];;
  if (u != q) {
    s->link[I] = q->link[I];;
    unset_link_null<I>::apply(s);
  }
  *linker = s;
  return q;
}

template <std::size_t I, typename T>
bst_node<T>* erase_node_one_null(bst_node<T>** linker, bst_node<T>* q) noexcept
{
  const std::size_t O = index_helper<I>::other;
  typedef bst_node<T>* node_pointer;
  node_pointer u = const_cast<node_pointer>(inorder_parent<O>(q));
  node_pointer s = q->link[O];
  if (u != q)
    s = u->link[I];
  s->link[I] = q->link[I];;
  if (has_null_link<O>::apply(s))
    set_link_null<I>::apply(u);
  else
    u->link[I] = s->link[O];;
  if (u != q) {
    s->link[O] = q->link[O];;
    unset_link_null<O>::apply(s);
  }
  *linker = s;
  return q;
}

template <std::size_t I, typename T>
bst_node<T>* erase_node(bst_node<T>* pq, bst_node<T>* q) noexcept
{
  // p is parent of q. We do not handle the case p = q
  // Returns the erased node to be released elsewhere.
  // WARNING: Still unfinished.
  const std::size_t O = index_helper<I>::other;
  bst_node<T>** linker = &pq->link[O];
  if (pq->link[I] == q)
    linker = &pq->link[I];
  if (!has_null_link<O>::apply(q) && !has_null_link<I>::apply(q))
    return erase_node_lr_non_null<I>(linker, q);

  if (has_null_link<O>::apply(q) && !has_null_link<I>::apply(q))
    return erase_node_one_null<O>(linker, q);

  if (!has_null_link<O>::apply(q) && has_null_link<I>::apply(q))
    return erase_node_one_null<I>(linker, q);

  if (pq->link[O] == q) { // Both links are null
    set_link_null<O>::apply(pq);
    pq->link[O] = q->link[O];
  } else {
    set_link_null<I>::apply(pq);
    pq->link[I] = q->link[I];
  }

  return q;
}

}

