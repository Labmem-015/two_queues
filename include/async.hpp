#pragma once

template <typename T>
requires (!std::is_void_v<T>)
struct Task {
    struct promise_type;
    using coro_handle_t = std::coroutine_handle<promise_type>;

    struct promise_type {
        auto get_return_object() -> coro_handle_t {
            return coro_handle_t::from_promise(*this);
        }
        
        std::suspend_never initial_suspend() noexcept {
            return {};
        }
        
        std::suspend_always final_suspend() noexcept {
            return {};
        }
        
        void return_value(T value) {
            std::print("Calling return_value in promise_object\n");
            this->value = value;
        }
        
        void unhandled_exception() {
            std::print("Unhandled exception caught in the coroutine!\n"); 
            std::terminate();
        }

        T value;
    };

    Task() = default;
    Task(const Task<T>&) = delete;
    Task(Task<T>&&) = default;

    Task<T>& operator=(const Task<T>&) = delete;
    Task<T>& operator=(Task<T>&&) = default;

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



template <typename T>
requires (!std::is_void_v<T>)
struct Awaitable {
    struct Awaiter {
        Awaiter(T value) : value(value) {}

        bool await_ready() {
            return false;
        }

        void await_suspend(Task<T>::coro_handle_t handle) noexcept {
            std::print("Setting coro_handle and suspend. tID: {}\n", std::this_thread::get_id());
        }

        T await_resume() noexcept {
            std::print("Resuming coroutine. tID: {}\n", std::this_thread::get_id());
            return value;
        }

    private:
        T value;
    };

    Awaitable(T value) : m_awaiter(value) {}

    Awaiter operator co_await() {
        return m_awaiter;
    }

private:
    Awaiter m_awaiter;
};
