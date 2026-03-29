#pragma once

int counter = 0;

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

    Task() {
        id = counter++;
        std::print("Default Task {} ctor\n", id);
    }

    Task(coro_handle_t handle) : coro_handle( handle) {
        id = counter++;
        std::print("Task {} has been created from coro_handle\n", id);
    }

    Task(const Task<T>&) = delete;
    Task(Task<T>&& other) : coro_handle(other.coro_handle) {
        id = counter++;
        std::print("Move Task {} ctor\n", id);
        other.coro_handle = nullptr;
    }

    Task<T>& operator=(const Task<T>&) = delete;
    Task<T>& operator=(Task<T>&& other) {
        std::print("Move Task {} assignment operator\n", id);
        if (coro_handle) {
            coro_handle.destroy();
            coro_handle = other.coro_handle;
            other.coro_handle = nullptr;
        } else {
            coro_handle = other.coro_handle;
            other.coro_handle = nullptr;
        }
        return *this;
    }

    ~Task() {
        std::print("Destoying Task {}\n", id);
        if(coro_handle) {
            coro_handle.destroy();
        }
    }
    
    T get_value() {
        return coro_handle.promise().value;
    }

    coro_handle_t coro_handle;
    int id = -1;
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
