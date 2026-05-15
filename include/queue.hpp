#pragma once

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
  Iterator &operator++() {
    if (!object) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
  }

  Iterator &operator--() {}

  T &operator*() { return *curr; }

  T *operator->() { return curr; }

private:
  struct Object {
    // TODO: Make those ptrs atomic
    Object *next = nullptr;
    Object *prev = nullptr;
    T data;
  } *object = nullptr;
};