#pragma once

template <typename T>
struct node {
  T key;
  node* llink;
  node* rlink;
  char tag;

  node()
  : llink(0)
  , rlink(0)
  , tag(0)
  {}
};

