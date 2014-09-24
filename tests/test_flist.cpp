#include <iostream>

#include "flist.hpp"

int main()
{
  flist<int> l(10);
  for (int i = 0; i < 4; ++i)
    if (!l.push_back(i))
      break;
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.remove_if(100);
  //l.remove_if(5);
  //l.remove_if(6);
  //l.remove_if(9);
  //l.remove_if(8);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.push_back(100);
  l.remove_if(100);
  //l.push_back(200);
  //l.push_back(300);
  std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, ","));
  std::cout << std::endl;
  return 0;
}

