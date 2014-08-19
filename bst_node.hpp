#pragma once

template <typename T>
struct node {
  T key;
  node* llink;
  node* rlink;
  char tag;

  node()
  : key(0)
  , llink(0)
  , rlink(0)
  , tag(0)
  {}
};

