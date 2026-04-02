#include "bit_manipulation/bit_enum/bit_enum.h"

namespace bit_manipulation::bit_enum {

std::vector<uint64_t> gray_code_sequence(unsigned num_bits) {
    if (num_bits == 0) return {0};
    const uint64_t count = uint64_t{1} << num_bits;
    std::vector<uint64_t> seq;
    seq.reserve(count);
    for (uint64_t i = 0; i < count; ++i) {
        seq.push_back(gray_encode(i));
    }
    return seq;
}

} // namespace bit_manipulation::bit_enum

