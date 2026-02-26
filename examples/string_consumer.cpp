#include <data_consumer.hpp>

int main(int argc, char* argv[]) {
    try{
        std::cout << "Start" << std::endl;
        auto requests_queue = std::make_shared<std::queue<Request*>>();;
        Consumer consumer;
        consumer.set_queue(requests_queue);
        std::string cmd;
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter 'q' or 'quit' to finish the program" << std::endl;
        while(cmd != "q" or cmd != "quit") {
            std::cout << "Enter command in format: <seconds_to_aio_wait> <some_text>" << std::endl;
            std::getline(std::cin, cmd);
            std::string_view view = cmd;
            if (cmd.empty()) {
                continue;
            }
            size_t pos = view.find_first_of(" ");
            if (pos == 0) {
                pos = view.find_first_of(" ");
            }
            if (pos == std::string::npos) {
                std::cout << "Invalid command, retry." << std::endl;
            }
            int seconds;
            auto fc_result = std::from_chars(view.data(), view.data() + pos + 1, seconds);
            if (fc_result.ec != std::errc()) {
                std::cout << "Error, while parsing seconds. Errc: " << (int)fc_result.ec << std::endl;
            }
            std::chrono::duration<int> duration{std::chrono::seconds{5}};
            
            auto payload = view.substr(pos);

            std::print("Before consume. tId: {}\n", std::this_thread::get_id());
            RequestTask task = consumer.consume();
            std::print("After consume. tId: {}\n", std::this_thread::get_id());
            
            // Simple processing...
            Request* req{requests_queue->front()};
            requests_queue->pop();
            if (req->coro_handle != task.coro_handle) {
                throw std::runtime_error("Desync requests processing");
            }
            memcpy(req->response, payload.data(), std::min(req->response_size, payload.size()));                
            if (req->coro_handle) {
                req->coro_handle.resume();
            } else {
                throw std::runtime_error("Coro handle is destroyed in simple processing routine");
            }
        }
    } catch (const std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown error occured" << std::endl;
    }
    std::cout << "Finish" << std::endl;
    return EXIT_SUCCESS;
}