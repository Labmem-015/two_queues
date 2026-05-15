struct Object {
  void print() { std::print("Hello, World\n"); }
  int value = 777;
};

template <typename T> struct iter {
  T *ptr = nullptr;
  T *operator->() const { return ptr; }
  T &operator*() const { return *ptr; }
};

int main(int, char *[]) {
  iter<Object> i;
  i.ptr = new Object;
  i->print();
  return i.ptr != std::addressof(*i);
}