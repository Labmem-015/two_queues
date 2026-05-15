#pragma once

namespace details {
template <typename T> struct Node {
  // TODO: Make those ptrs atomic
  Node *next = nullptr;
  Node *prev = nullptr;
  T *data = nullptr;
};
} // namespace details

template <typename T> class Queue {
public:
  class Iterator;

  Iterator &begin() { return head; };

  Iterator &end() { return tail; };

  void pop() {};

private:
  Iterator head;
  Iterator tail;
};

template <typename T> class Queue<T>::Iterator {
public:
  Iterator() = default;
  Iterator &operator++() {
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->next) {
      throw std::runtime_error("Out of bound in Queue<T>::Iterator!");
    }
    storage = storage->next;
  }

  Iterator &operator--() {
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->prev) {
      throw std::runtime_error("Out of bound in Queue<T>::Iterator!");
    }
    storage = storage->prev;
  }

  T &operator*() { return *(storage->data); }

  T *operator->() { return storage->data; }

private:
  friend class Queue<T>;
  details::Node<T> *storage = nullptr;
};