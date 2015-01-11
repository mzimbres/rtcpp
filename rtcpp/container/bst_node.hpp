#pragma once

  /*

  Definition of a node for a threaded binary search tree and its fundamental
  operations.

  */

namespace rt {

template <typename T>
struct bst_node {
  typedef T value_type;
  bst_node* llink;
  bst_node* rlink;
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

template <typename T>
int has_null_llink(const bst_node<T>* p) noexcept
{
  return p->tag & detail::lbit;
}

template <typename T>
int has_null_rlink(const bst_node<T>* p) noexcept
{
  return p->tag & detail::rbit;
}

template <typename T>
bst_node<T>* inorder_successor(const bst_node<T>* p) noexcept
{
  if (has_null_rlink(p))
    return p->rlink;

  bst_node<T>* q = p->rlink;
  while (!has_null_llink(q))
    q = q->llink;

  return q;
}

template <typename T>
bst_node<T>* inorder_predecessor(const bst_node<T>* p) noexcept
{
  if (has_null_llink(p))
    return p->llink;

  bst_node<T>* q = p->llink;
  while (!has_null_rlink(q))
    q = q->rlink;

  return q;
}

template <typename T>
bst_node<T>* preorder_successor(const bst_node<T>* p) noexcept
{
  if (!has_null_llink(p))
    return p->llink;

  if (!has_null_rlink(p))
    return p->rlink;

  // This is a leaf node.
  bst_node<T>* q = p->rlink;
  while (has_null_rlink(q))
    q = q->rlink;

  return q->rlink;
}

template <typename T>
void attach_node_left(bst_node<T>* p, bst_node<T>* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->llink = p->llink;
  q->tag = has_null_rlink(q) | has_null_llink(p);
  p->llink = q;
  p->tag = has_null_rlink(p);
  q->rlink = p;
  set_rlink_null(q);

  if (!has_null_llink(q)) {
    bst_node<T>* qs = inorder_predecessor(q);
    qs->rlink = q;
  }
}

template <typename T>
void attach_node_right(bst_node<T>* p, bst_node<T>* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->rlink = p->rlink;
  q->tag = has_null_llink(q) | has_null_rlink(p);
  p->rlink = q;
  p->tag = has_null_llink(p);
  q->llink = p;
  set_llink_null(q);

  if (!has_null_rlink(q)) {
    bst_node<T>* qs = inorder_successor(q);
    qs->llink = q;
  }
}

}

