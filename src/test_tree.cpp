#include <array>
#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>

#include "rtcpp.hpp"
#include "test.hpp"

template <class Successor>
void traversal_tester( const std::vector<int>& input
                     , const std::vector<int>& expected)
{
  rt::bst_node root {};
  for (auto o : input)
    rt::bst_insert(root, o);

  using iter = rt::bst_iter<Successor>;

  auto b = std::equal( iter {root.left}
                     , iter {}
                     , std::begin(expected));
  if (!b)
    throw std::runtime_error("traversal_tester");
}

RT_TEST(test_bst_inorder)
{
  (void)f;
  std::vector<int> v {20, 3, 2, 8, 5};

  rt::bst_node root {};
  for (auto o : v)
    bst_insert(root, o);

  std::cout << "test_bst_inorder" << std::endl;
  std::cout << "inorder_recursive" << std::endl;
  rt::inorder_recursive(root.left);
  std::cout << "inorder_traversal" << std::endl;
  inorder_traversal(root.left);
  std::cout << "inorder_traversal2" << std::endl;
  inorder_traversal2(root.left);
  std::cout << "inorder_traversal3" << std::endl;

  using iter = rt::bst_iter<rt::inorder_successor>;
  std::for_each( iter {root.left}, iter {}
               , [](auto o){std::cout << o << std::endl;});
  std::cout << "inorder_traversal4" << std::endl;
  std::copy( iter {root.left}, iter {}
           , std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  traversal_tester<rt::inorder_successor>({}, {});

  traversal_tester<rt::inorder_successor>( {6, 3, 5, 2, 4, 1}
                                         , {1, 2, 3, 4, 5, 6});
}

void test_bst_preorder()
{
  std::cout << "test_bst_preorder" << std::endl;

  std::vector<int> v {20, 3, 2, 8, 5};

  rt::bst_node root {};
  for (auto o : v)
    bst_insert(root, o);

  std::cout << "preorder_recursive" << std::endl;
  rt::preorder_recursive(root.left);

  std::cout << "preorder_traversal" << std::endl;
  preorder_traversal(root.left);

  std::cout << "preorder_traversal2" << std::endl;
  preorder_traversal2(root.left);

  traversal_tester<rt::preorder_successor>({}, {});

  traversal_tester<rt::preorder_successor>( {8, 3, 2, 7, 5, 9}
                                          , {8, 3, 2, 7, 5, 9});

  traversal_tester<rt::preorder_successor>( {1, 2, 3, 4, 5, 6}
                                          , {1, 2, 3, 4, 5, 6});

  traversal_tester<rt::preorder_successor>( {6, 5, 4, 3, 2, 1}
                                          , {6, 5, 4, 3, 2, 1});
}

void test_bst_postorder()
{
  std::cout << "test_bst_postorder" << std::endl;

  std::vector<int> v {20, 3, 2, 8, 5};

  rt::bst_node root {};
  for (auto o : v)
    bst_insert(root, o);

  std::cout << "postorder_recursive" << std::endl;
  rt::postorder_recursive(root.left);
  std::cout << "postorder_traversal" << std::endl;
  rt::postorder_traversal(root.left);

  //traversal_tester<rt::postorder_successor>({}, {});

  //traversal_tester<rt::postorder_successor>( {8, 3, 2, 7, 5, 9}
  //                                         , {2, 5, 7, 3, 9, 8});

  //traversal_tester<rt::postorder_successor>( {1, 2, 3, 4, 5, 6}
  //                                         , {6, 5, 4, 3, 2, 1});

}

void test_bst_copy()
{
  std::cout << "test_bst_copy" << std::endl;

  std::vector<int> v {20, 3, 2, 8, 5};

  rt::bst_node from {};
  for (auto o : v)
    bst_insert(from, o);

  rt::bst_node to {};
  copy(&from, &to);

  using preorder_iter = rt::bst_iter<rt::preorder_successor>;
  auto b = std::equal( preorder_iter {from.left}
                     , preorder_iter {}
                     , preorder_iter {to.left});
  if (!b)
    throw std::runtime_error("test_bst_copy");

  using inorder_iter = rt::bst_iter<rt::inorder_successor>;
  b = std::equal( inorder_iter {from.left}
                     , inorder_iter {}
                     , inorder_iter {to.left});
  if (!b)
    throw std::runtime_error("test_bst_copy");

  //using postorder_iter = rt::bst_iter<rt::postorder_successor>;
  //auto b = std::equal( postorder_iter {from.left}
  //                   , postorder_iter {}
  //                   , postorder_iter {to.left});
  //if (!b)
  //  throw std::runtime_error("test_bst_copy");
}

int main()
{
  try {
    test_bst_preorder();
    test_bst_inorder();
    test_bst_postorder();
  } catch (...) {
    std::cerr << "Error." << std::endl;
    return 1;
  }
  return 0;
}

