#include <atomic_queue.hpp>

using namespace std::chrono;

auto get_time_interval_ms() {
  using int_distro_t = std::uniform_int_distribution<std::mt19937::result_type>;
  static std::random_device rd_;
  static std::mt19937 rng{rd_()};
  int_distro_t int_genor{1, 100};
  return milliseconds{int_genor(rng)};
}

struct Payload {
  std::thread::id id;
  size_t log_id;
  steady_clock::time_point time;
  std::string message;
};

void loop(int, char *[]);
void simple();

int main(int argv, char *argc[]) {
  simple();
  // loop(argv, argc);
  return 0;
}

void simple() {
  std::print("Start\n");
  AtomicQueue<Payload> mpsc;
  for (int i = 0; i < 3; ++i) {
    Payload data;
    data.log_id = i;
    mpsc.push(data);
  }
  for (int i = 0; i < 4; ++i) {
    try {

      auto data = mpsc.pop();
      std::print("i: {}\n", data.log_id);
    } catch (const std::exception &e) {
      std::print("Catched exception. What: {}\n", e.what());
    }
  }
}

void loop(int argv, char *argc[]) {
  int thr_num = 2;
  if (argv == 2) {
    thr_num = std::stoi(argc[1]);
  }

  auto start = steady_clock::now();
  AtomicQueue<Payload> mpsc;
  std::vector<std::jthread> threads;
  std::atomic_size_t log_id{0};
  for (int i = 0; i < thr_num; i++) {
    threads.emplace_back([&mpsc, &log_id, &start](std::stop_token stop) {
      while (!stop.stop_requested()) {
        auto end = steady_clock::now();
        auto diff = duration_cast<microseconds>(end - start).count();
        Payload data{
            .id = std::this_thread::get_id(), .log_id = log_id++, .time = end};
        data.message = std::format(
            "This message is from thread {}.\tlog_id={}.\tGenTime: {}mcs",
            data.id, data.log_id, diff);
        mpsc.push(data);
      }
    });
  }

  std::jthread consumer{[&mpsc](std::stop_token) {
    auto data = mpsc.pop();
    auto diff = duration_cast<microseconds>(steady_clock::now() - data.time);
    std::print("Received data in {}mcs. {}", diff.count(), data.message);
  }};
  std::this_thread::sleep_for(1s);
}