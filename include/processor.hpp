#pragma once

#include "consumer.hpp"
#include "producer.hpp"
#include "queue.hpp"

/**
 * We need to produce requests with a pointers to provided (allocated) buffers.
 * Those requests should be incouming from consumer.
 * Producer generates data, which will be written in consumer provided buffer.
 *
 * By condition of the problem we must copy data
 * from producer's queue to consumer's queue.
 */

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
  Queue<Data *> m_data_queue;
  Queue<Request *> m_request_queue;
};