#include <data_producer.hpp>


int main(int argc, char* argv[]) {
    std::cout << "Start" << std::endl;
    Producer generator;
    int num_of_strings = 10;
    int string_size = 25;
    if (argc > 1) {
        num_of_strings = std::stoi(argv[1]);
    }
    if (argc > 2) {
        string_size = std::stoi(argv[2]);
    }
    for (int i = 0; i < num_of_strings; ++i) {
        std::string generated_string = generator.generate_string(string_size);
        std::print("{}\n", generated_string);
    }
    std::cout << "Finish" << std::endl;
    return EXIT_SUCCESS;
}