#pragma once

/**
 * We need to produce requests with a pointers to provided (allocated) buffers.
 * Those requests should be incouming from consumer.
 * Producer generates data, which will be written in consumer provided buffer.
 *
 * By condition of the problem we must copy data
 * from producer's queue to consumer's queue.
 */

#include "consumer.hpp"
#include "producer.hpp"

// TODO: Implement lockfree queue here
template<typename T>
struct Node;

template<typename T>
struct Queue {
    using node_t = Node<T>;
    std::atomic<node_t*> head;
    std::atomic<node_t*> tail;
};

class Processor {
public:
  void process() {
    while (true) {
      const auto &it = m_data_queue.begin();
      m_data_queue.pop();
    }
  }

  void push_data(Data *data) {};
  void push_request(Request *req) {};

private:
    std::queue<Data*> m_data_queue;
    std::queue<Request*> m_request_queue;
};