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

template <typename Node>
Node* inorder_successor(Node* p) noexcept
{
  if (has_null_rlink(p))
    return p->rlink;

  Node* q = p->rlink;
  while (!has_null_llink(q))
    q = q->llink;

  return q;
}

template <typename Node>
Node* inorder_predecessor(Node* p) noexcept
{
  if (has_null_llink(p))
    return p->llink;

  Node* q = p->llink;
  while (!has_null_rlink(q))
    q = q->rlink;

  return q;
}

template <typename Node>
Node* preorder_successor(Node* p) noexcept
{
  if (!has_null_llink(p))
    return p->llink;

  if (!has_null_rlink(p))
    return p->rlink;

  // This is a leaf node.
  Node* q = p->rlink;
  while (has_null_rlink(q))
    q = q->rlink;

  return q->rlink;
}

template <typename Node>
void attach_node_left(Node* p, Node* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->llink = p->llink;
  q->tag = has_null_rlink(q) | has_null_llink(p);
  p->llink = q;
  p->tag = has_null_rlink(p);
  q->rlink = p;
  set_rlink_null(q);

  if (!has_null_llink(q)) {
    Node* qs = inorder_predecessor(q);
    qs->rlink = q;
  }
}

template <typename Node>
void attach_node_right(Node* p, Node* q) noexcept
{
  // Attaches node q on the left of p. Does not check if pointers are valid.
  q->rlink = p->rlink;
  q->tag = has_null_llink(q) | has_null_rlink(p);
  p->rlink = q;
  p->tag = has_null_llink(p);
  q->llink = p;
  set_llink_null(q);

  if (!has_null_rlink(q)) {
    Node* qs = inorder_successor(q);
    qs->llink = q;
  }
}

}

