#pragma once

template <typename T>
requires (!std::is_void_v<T>)
struct Task {
    struct Promise;
    using promise_type = Promise;
    using coro_handle_t = std::coroutine_handle<promise_type>;
    struct Promise {
        auto get_return_object() -> std::coroutine_handle<Promise> {
            return std::coroutine_handle<Promise>::from_promise(*this);
        }
        
        std::suspend_never initial_suspend() noexcept {
            return {};
        }
        
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        
        void return_value(T value) {
            std::print("Calling return_void in promise_object\n");
            this->value = value;
        }
        
        void unhandled_exception() {
            std::print("Unhandled exception caught in the coroutine!\n"); 
            std::terminate();
        }

        T value;
    };

    Task(coro_handle_t handle) : coro_handle( handle) {
        std::print("Task has been created\n");
    }

    ~Task() {
        std::print("Destoying target\n");
        if(coro_handle) {
            coro_handle.destroy();
        }
        std::print("Target has been destroyed\n");
    }
    
    T get_value() {
        return coro_handle.promise().value;
    }

    coro_handle_t coro_handle;
};

struct Request;
using RequestTask = Task<Request*>;
struct Request {
    char* response;
    size_t response_size = 0;
    RequestTask::coro_handle_t coro_handle;
    std::atomic_bool is_ready = false;
    bool is_last = false;

    Request() = default;
    Request(const Request&) = delete;
    Request(Request&&) = default;
};

struct AsyncResponse {
    struct Awaiter {
        Awaiter(Request* req) : request(req) {}

        bool await_ready() {
            return false;
        }

        void await_suspend(RequestTask::coro_handle_t handle) noexcept {
            request->coro_handle = handle;
            std::print("Setting coro_handle and suspend. tID: {}\n", std::this_thread::get_id());
        }

        Request* await_resume() noexcept {
            std::print("Resuming coroutine. tID: {}\n", std::this_thread::get_id());
            return request;
        }

    private:
        Request* request;
    };

    AsyncResponse(Request* req) : m_awaiter(req) {}

    Awaiter operator co_await() {
        return m_awaiter;
    }

private:
    Awaiter m_awaiter;
};

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