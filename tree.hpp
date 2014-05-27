#include <iostream>

template <typename T>
struct node {
  T key;
  node* llink;
  node* rlink;
};

template <typename T, std::size_t N>
class bst {
  static_assert(N > 0, "list: size not allowed.");
  public:
  typedef std::size_t size_type;
  typedef node<T> node_type;
  typedef node_type* node_pointer;
  private:
  node_type pool[N];
  node<T>* root;
  node<T>* avail;
  node_pointer add_node(T key)
  {
    if (!avail)
      return 0; // The tree has exhausted its capacity.

    node_pointer q = avail;
    avail = avail->llink;
    q->key = key;
    q->llink = 0;
    q->rlink = 0;
    return q;
  }
  public:
  bst()
  : root(0)
  , avail(0)
  {
    // Let us link the avail stack.
    pool[0].llink = 0;
    pool[0].rlink = 0;
    for (size_type i = 1; i < N; ++i) {
      pool[i].llink = &pool[i - 1];
      pool[i].rlink = 0;
    }
    avail = &pool[N - 1];
  }
  node_pointer insert(T key)
  {
    if (!root) { // the tree is empty
      root = avail;
      avail = avail->llink;
      root->key = key;
      return root;
    }
    // We have enough space lets insert the element.
    node_pointer p = root;
    for (;;) {
      if (key < p->key) {
        if (p->llink) {
          p = p->llink;
          continue;
        }
        p->llink = add_node(key);
        return p->llink;
      } else if (key > p->key) {
        if (p->rlink) {
          p = p->rlink;
          continue;
        }
        p->rlink = add_node(key);
        return p->rlink;
      } else {
        return p;
      }
    }
  }
};

