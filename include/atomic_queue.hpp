#pragma once

namespace details {
template <typename T> struct Node {
  // TODO: Make those ptrs atomic
  std::atomic<Node *> next = nullptr;
  std::atomic<Node *> prev = nullptr;
  T *data = nullptr;
};
} // namespace details

template <typename T> class AtomicQueue {
public:
  void push(const T &value) {};
  T pop() { return {}; }
};