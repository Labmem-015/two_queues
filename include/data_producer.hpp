/**
 * We need to produce packages with provided (allocated) buffers.
 * Those packages should be considered as requests from consumer with a pointer to a buffer
 * where payload will be written.
 * 
 * Also we have to produce payloads. And by condition of the problem we must copy data
 * from producer queue to consumer queue. 
 */
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
    m_special_char_gen(32, 47) {}

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
};