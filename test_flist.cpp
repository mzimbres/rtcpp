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
  void remove_if(T value)
  {
    node_pointer* p1 = &head;
    node_pointer p2 = head->llink;
    while (p2) {
      if (p2->info == value) {
        node_pointer tmp = p2->llink;
        p2->llink = avail;
        avail = p2;
        p2 = tmp;
        *p1 = p2;
        continue;
      }
      tail = p2;
      p1 = &p2->llink;
      p2 = p2->llink;
    }
  }
};

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

