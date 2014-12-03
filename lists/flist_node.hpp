#pragma once

namespace rtcpp {

template <typename T>
struct flist_node {
  T info;
  flist_node* llink;
};

}

