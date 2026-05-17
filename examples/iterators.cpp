#include <queue.hpp>

int main(int argv, char *argc[]) {
  int size = 10;
  if (argv == 2) {
    size = std::stoi(argc[1]);
  }
  std::vector<int> vec;
  vec.resize(size);
  std::ranges::iota(vec, 0);

  Queue<int> queue;
  for (int num : vec) {
    queue.push(num);
  }

  auto qit = queue.begin();
  for (int num : vec) {
    assert(num == *qit);
    ++qit;
  }
  std::print("Size of queue is: {}\n", queue.size());
  for (int i = 0; i < vec.size() / 2; ++i) {
    queue.pop();
  }
  std::print("Size of queue left: {}\n", queue.size());

  qit = queue.begin();
  do {
    std::cout << *qit << " ";
    static int pos_in_chunk = 1;
    if (pos_in_chunk++ % 8 == 0) {
      pos_in_chunk = 1;
      std::cout << std::endl;
    }
  } while (++qit);
  std::cout << std::endl << std::endl;

  return 0;
}