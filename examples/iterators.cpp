#include <queue.hpp>

struct Object {
  int var = 77;
};

int main(int, char *[]) {
  std::vector<int> vec;
  auto it = vec.front();
  Queue<Object> q;
  // iter<Object> i;
  // i.ptr = new Object;
  // i->print();
  // return i.ptr != std::addressof(*i);
}