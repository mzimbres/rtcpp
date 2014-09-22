#pragma once

template <typename T>
struct node {
  T key;
  node* llink;
  node* rlink;
  int tag;

  node()
  : key(0)
  , llink(0)
  , rlink(0)
  , tag(0)
  {}
};

