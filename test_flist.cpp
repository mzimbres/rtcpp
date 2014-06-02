#include <iostream>
#include <vector>
#include <iterator>

template <typename T>
struct flist_node {
  T info;
  flist_node* llink;
};

template <typename T>
class flist_iterator : public std::iterator<std::forward_iterator_tag, T> {
  public:
  typedef flist_node<T> node_type;
  typedef node_type* node_pointer;
  private:
  node_pointer head;
  public:
  flist_iterator(node_pointer h)
  : head(h)
  {}
  flist_iterator& operator++()
  {
    head = head->llink;
  }
  flist_iterator operator++(int)
  {
    flist_iterator tmp(*this);
    operator++();
    return tmp;
  }
  T operator*() const { return head->info; }
  bool operator==(const flist_iterator<T>& rhs) {return head == rhs.head;}
  bool operator!=(const flist_iterator<T>& rhs) {return !(*this == rhs);}
};

template <typename T>
class flist {
  public:
  typedef flist_node<T> node_type;
  typedef node_type* node_pointer;
  typedef flist_iterator<T> iterator;
  private:
  std::vector<flist_node<T>> pool;
  node_pointer avail;
  node_pointer head;
  node_pointer tail;
  public:
  iterator begin() {return iterator(head);}
  iterator end() {return iterator(0);}
  flist(std::size_t n)
  : pool(n)
  , avail(0)
  , head(0)
  , tail(0)
  {
    pool[0].llink = 0;
    for (std::size_t i = 0; i < n; ++i)
      pool[i].llink = &pool[i - 1];
    avail = &pool.back();
  }
  node_pointer add_node(T data)
  {
    if (!avail)
      return 0;

    node_pointer q = avail;
    avail = avail->llink;
    q->info = data;
    q->llink = 0;
    return q;
  }
  bool push_back(T data)
  {
    if (!head) {
      head = add_node(data);
      tail = head;
      return true;
    }
    const node_pointer q = add_node(data);
    if (!q)
      return false;
    tail->llink = q;
    tail = q;
    return true;
  }
};

int main()
{
  flist<int> l(10);
  for (int i = 0; i < 10; ++i)
    if (!l.push_back(i))
      break;
  std::copy(std::begin(l), std::end(l), std::ostream_iterator<int>(std::cout, ","));
  std::cout << std::endl;
  return 0;
}
