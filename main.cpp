#include <processor.hpp>

int main(int argc, char *argv[]) {
  // spawn three threads: producer, consumer, lockfree processor
  // in those thread we call `produce()` and `consume` and place return values
  // to lockfree queues of the lockfree processor
  Producer producer;
  Consumer consumer;
  Processor processor;
  std::jthread thr1([&](std::stop_token stop) {
    while (!stop.stop_requested()) {
      auto *data = producer.produce();
      processor.push_data(data);
    }
  });
  std::jthread thr2([&](std::stop_token stop) {
    while (!stop.stop_requested()) {
      auto request = consumer.create_request();
      processor.push_request(request);
    }
  });
  processor.process();
  return EXIT_SUCCESS;
}