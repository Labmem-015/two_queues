#pragma once

#include <requests_system.hpp>

class Consumer {
public:
    [[nodiscard]] AsyncResponse make_request(){
        Request *req = new Request;
        req->response = new char[m_response_size];
        req->response_size = m_response_size;
        AsyncResponse async_response{req};
        m_queue_of_sent->push(req);
        return async_response;
    }

    [[maybe_unused]] RequestTask consume() {
        Request* response = co_await make_request();
        std::string_view result{response->response, response->response_size};
        std::print("Response is: {}. tId: {}\n", result, std::this_thread::get_id());
        delete response;
    }
    
    void set_queue(std::shared_ptr<std::queue<Request*>> queue) {
        m_queue_of_sent = queue;
    }
private:
    std::shared_ptr<std::queue<Request*>> m_queue_of_sent;
    static constexpr const size_t m_response_size = 10;
    std::atomic_bool is_running = true;
};