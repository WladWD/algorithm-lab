#include "algorithms/sorting/radix_sort/radix_sort.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

namespace algorithms::sorting::radix_sort {

namespace {

constexpr int kBits  = 8;
constexpr int kBase  = 1 << kBits;       // 256
constexpr uint64_t kMask = kBase - 1;    // 0xFF

// Counting-sort by a single byte at bit offset `shift`.
void counting_sort_byte(std::vector<uint64_t>& data,
                        std::vector<uint64_t>& buf,
                        int shift) {
    std::array<std::size_t, kBase> count{};

    for (auto v : data) {
        ++count[(v >> shift) & kMask];
    }

    // Prefix sums.
    std::partial_sum(count.begin(), count.end(), count.begin());

    // Place in reverse for stability.
    for (auto it = data.rbegin(); it != data.rend(); ++it) {
        auto digit = (*it >> shift) & kMask;
        buf[--count[digit]] = *it;
    }

    std::swap(data, buf);
}

} // namespace

void radix_sort(std::vector<uint64_t>& data) {
    if (data.size() <= 1) return;

    // Find the actual max to skip unnecessary passes.
    uint64_t max_val = *std::max_element(data.begin(), data.end());

    std::vector<uint64_t> buf(data.size());

    for (int shift = 0; shift < 64 && (max_val >> shift) > 0; shift += kBits) {
        counting_sort_byte(data, buf, shift);
    }
}

void radix_sort_signed(std::vector<int64_t>& data) {
    if (data.size() <= 1) return;

    // Offset all values to make them non-negative.
    int64_t min_val = *std::min_element(data.begin(), data.end());

    std::vector<uint64_t> unsigned_data(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        unsigned_data[i] = static_cast<uint64_t>(data[i] - min_val);
    }

    radix_sort(unsigned_data);

    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<int64_t>(unsigned_data[i]) + min_val;
    }
}

} // namespace algorithms::sorting::radix_sort

