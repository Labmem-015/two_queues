#pragma once

#include <async.hpp>

class Consumer {
public:
    [[maybe_unused]] RequestTask deferred_consume(Request* req) {
        // async wait for request to be resumed in processor
        Request* response = co_await Awaitable(req);
        std::string_view result{response->buffer, response->buffer_size};
        std::print("Response is: {}. tId: {}\n", result, std::this_thread::get_id());
        delete response;
    }

    Request* create_request() {
        Request *req = new Request;
        req->buffer = new char[m_response_size];
        req->buffer_size = m_response_size;
        auto task = deferred_consume(req);
        m_tasks.emplace(task);
        return req;
    }

    void set_queue(std::shared_ptr<std::queue<Request*>> queue) {
        m_queue_of_sent = queue;
    }
private:
    std::shared_ptr<std::queue<Request*>> m_queue_of_sent;
    static constexpr const size_t m_response_size = 10;
    std::atomic_bool m_is_running = true;
    std::queue<RequestTask> m_tasks;
};