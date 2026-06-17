#pragma once

namespace details {
template <typename T> struct Node {
  // TODO: Make those ptrs secured with mutexes
  Node *next = nullptr;
  Node *prev = nullptr;
  T *data = nullptr;
};
} // namespace details

template <typename T> class Queue {
public:
  class Iterator;

  Queue() = default;

  Iterator &begin() { return head; };

  Iterator &end() { return none; };

  T &front() { return *head; }

  T &back() { return *(tail.storage); }

  void pop() noexcept {
    if (!head.storage) {
      return;
    }
    auto *next = head.storage->next;
    auto *del = head.storage;
    if (next) {
      next->prev = nullptr;
    }
    head.storage = next;
    delete del->data;
    delete del;
    --m_size;
  }

  void push(const T &value) noexcept {
    auto *new_node = new details::Node<T>();
    new_node->data = new T(value);
    if (head.storage == nullptr) [[unlikely]] {
      head.storage = new_node;
      tail.storage = new_node;
    } else [[likely]] {
      new_node->prev = tail.storage;
      tail.storage->next = new_node;
      tail.storage = new_node;
    }
    ++m_size;
  }

  void push(T &&value) noexcept {
    auto *new_node = new details::Node<T>();
    new_node->data = new T(std::move(value));
    if (head.storage == nullptr) [[unlikely]] {
      head.storage = new_node;
      tail.storage = new_node;
    } else [[likely]] {
      new_node->prev = tail.storage;
      tail.storage->next = new_node;
      tail.storage = new_node;
    }
    ++m_size;
  }

  template <typename... Args> void emplace(Args... args) {};

  size_t size() const noexcept { return m_size; }

private:
  size_t m_size = 0;
  Iterator head;
  Iterator tail;
  Iterator none;
};

template <typename T> class Queue<T>::Iterator {
public:
  Iterator() = default;

  virtual ~Iterator() = default;

  bool operator++() {
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->next) {
      return false;
    }
    storage = storage->next;
    return true;
  }

  bool operator++(int) { // postfix operator
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->next) {
      return false;
    }
    storage = storage->next;
    return true;
  }

  bool operator--() {
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->prev) {
      return false;
    }
    storage = storage->prev;
    return true;
  }

  bool operator--(int) { // postfix operator
    if (!storage) {
      throw std::runtime_error("Object is nullptr in Queue<T>::Iterator!");
    }
    if (!storage->prev) {
      return false;
    }
    storage = storage->prev;
    return true;
  }

  bool operator==(const Iterator &other) { return storage == other.storage; }

  bool operator!=(const Iterator &other) { return storage != other.storage; }

  T &operator*() { return *(storage->data); }

  T *operator->() { return storage->data; }

private:
  friend class Queue<T>;
  details::Node<T> *storage = nullptr;
};