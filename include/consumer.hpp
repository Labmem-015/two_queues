#pragma once

#include <async.hpp>

struct Request {
  char *buffer{nullptr};
  size_t buffer_size{0};
  Task<Request *> task;
  std::atomic_bool is_ready{false};
  bool is_last{false};

  Request() = default;
  Request(const Request &) = delete;
  Request(Request &&other) {
    buffer = other.buffer;
    other.buffer = nullptr;
    buffer_size = other.buffer_size;
    other.buffer_size = 0;
    task = std::move(other.task);
    // TODO: think about memory order
    is_ready = other.is_ready.load();
    is_last = other.is_last;
  };
};

class Consumer {
  Task<Request *> deferred_consume(Request *req) {
    // async wait for request to be resumed in processor
    Request *response = co_await Awaitable(req);
    std::string_view result{response->buffer, response->buffer_size};
    std::print("Response is: {}. tId: {}\n", result,
               std::this_thread::get_id());
    delete response;
    co_return nullptr; // crutch
  }

public:
  Request *create_request() {
    Request *req = new Request;
    req->buffer = new char[m_response_size];
    req->buffer_size = m_response_size;
    req->task = deferred_consume(req);
    return req;
  }

private:
  static constexpr const size_t m_response_size = 10;
};