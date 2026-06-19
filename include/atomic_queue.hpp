#pragma once

namespace details {
template <typename T> struct Node {
  Node<T> *next = nullptr;
  Node<T> *prev = nullptr;
  T data;
  Node(const T &data) : data(data) {}
  Node(T &&data) : data(std::move(data)) {}
};
} // namespace details

template <typename T> class AtomicQueue {
  using Node = details::Node<T>;

public:
  void push(const T &value) { impl_push(new Node{value}); }

  void push(T &&value) { impl_push(new Node{std::move(value)}); }

  size_t get_size() {
    bool expected = false;
    while (!m_busy.compare_exchange_weak(expected, true,
                                         std::memory_order::acquire)) {
      expected = false;
    }
    return m_size.load(std::memory_order::relaxed);
    m_busy.store(false, std::memory_order::release);
  }

  T pop() {
    // If queue is empty, then UB
    bool expected = false;
    while (!m_busy.compare_exchange_weak(expected, true,
                                         std::memory_order::acquire)) {
      expected = false;
    }
    if (!m_size) {
      m_busy.store(false, std::memory_order::release);
      throw std::runtime_error{"AtomicQueue is empty!"};
    }
    auto *node = m_head.load(std::memory_order::relaxed);
    auto *new_head = node->next;
    if (new_head) {
      new_head->prev = nullptr;
    } else {
      m_tail.store(new_head, std::memory_order::relaxed);
    }
    m_head.store(new_head, std::memory_order::relaxed);
    m_size.fetch_sub(1, std::memory_order::relaxed);
    m_busy.store(false, std::memory_order::release);
    auto ret = std::move(node->data);
    delete node;
    return ret;
  }

  ~AtomicQueue<T>() noexcept {
    while (get_size()) {
      pop();
    }
  }

private:
  void impl_push(Node *new_node) {
    bool expected = false;
    while (!m_busy.compare_exchange_weak(expected, true,
                                         std::memory_order::acquire)) {
      expected = false;
    }
    if (m_size.load(std::memory_order::relaxed)) [[likely]] {
      auto *old = m_tail.load();
      old->next = new_node;
      new_node->prev = old;
      m_tail.store(new_node);
    } else [[unlikely]] {
      m_head.store(new_node, std::memory_order::relaxed);
      m_tail.store(new_node, std::memory_order::relaxed);
    }
    m_size.fetch_add(1, std::memory_order::relaxed);
    m_busy.store(false, std::memory_order::release);
  }

private:
  std::atomic<Node *> m_head{nullptr};
  std::atomic<Node *> m_tail{nullptr};
  std::atomic_bool m_busy{false};
  std::atomic_size_t m_size{0};
};
