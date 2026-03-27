#pragma once

struct Data {
    size_t total_size = 0;
    size_t str1_offset = 0;
    size_t str2_offset = 0;
};


class Producer {
    enum class ASCIIType : int8_t {
        DIGIT = 0,
        LOWER_CASE_LETTER,
        UPPER_CASE_LETTER,
        SPECIAL_CHARACTER,
        _COUNT
    };

    using int_distro_t = std::uniform_int_distribution<std::mt19937::result_type>;

public:
    Producer() :
    m_rng(m_rd_()),
    m_ascii_type_gen(0, (int8_t)ASCIIType::_COUNT - 1),
    m_digit_gen(0, 9),
    m_lower_case_gen('a', 'z'),
    m_upper_case_gen('A', 'Z'),
    m_special_char_gen(32, 47),
    m_length_gen(1,25) {}
    
    Data* produce() {
        auto str1_len = m_length_gen(m_rng);
        auto str2_len = m_length_gen(m_rng);
        Data* data = static_cast<Data*>(malloc(sizeof(Data) + str1_len + str2_len));
        data->total_size + str1_len + str2_len;
        data->str1_offset = sizeof(Data);
        data->str2_offset = sizeof(Data) + str2_len;
        std::string_view str1 = generate_string(str1_len);
        std::string_view str2 = generate_string(str2_len);
        memcpy(data + data->str1_offset, str1.data(), str1.size());
        memcpy(data + data->str2_offset, str2.data(), str2.size());
        return data;
    }

    std::string generate_string(size_t len) {
        std::string output_str;
        output_str.reserve(len);
        for (int i = 0; i < len; ++i) {
            char c;
            auto type = m_ascii_type_gen(m_rng);
            switch ((ASCIIType)type)
            {
                case ASCIIType::DIGIT:
                c = m_digit_gen(m_rng);
            break;
                case ASCIIType::LOWER_CASE_LETTER:
                c = m_lower_case_gen(m_rng);
                break;
            case ASCIIType::UPPER_CASE_LETTER:
                c = m_upper_case_gen(m_rng);
                break;
            case ASCIIType::SPECIAL_CHARACTER:
                c = m_special_char_gen(m_rng);
                break;
            default:
                throw std::runtime_error(std::format("Invalid char in {}", __func__));
                break;
            }
            output_str.push_back(c);
        }
        return output_str;
    }
    
private:
    std::random_device m_rd_;
    std::mt19937 m_rng;
    int_distro_t m_ascii_type_gen;
    int_distro_t m_digit_gen;
    int_distro_t m_lower_case_gen;
    int_distro_t m_upper_case_gen;
    int_distro_t m_special_char_gen;
    int_distro_t m_length_gen;

};