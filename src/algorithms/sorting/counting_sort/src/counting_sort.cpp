#include "algorithms/sorting/counting_sort/counting_sort.h"

#include <algorithm>
#include <numeric>

namespace algorithms::sorting::counting_sort {

void counting_sort(std::vector<int64_t>& data, int64_t max_val) {
    if (data.size() <= 1) return;

    const auto k = static_cast<std::size_t>(max_val + 1);
    std::vector<std::size_t> count(k, 0);

    // Count occurrences.
    for (auto v : data) {
        ++count[static_cast<std::size_t>(v)];
    }

    // Prefix sums (cumulative counts) for stable placement.
    std::vector<std::size_t> prefix(k, 0);
    std::partial_sum(count.begin(), count.end(), prefix.begin());

    // Build sorted output (traverse in reverse for stability).
    std::vector<int64_t> output(data.size());
    for (auto it = data.rbegin(); it != data.rend(); ++it) {
        auto idx = static_cast<std::size_t>(*it);
        output[--prefix[idx]] = *it;
    }

    data = std::move(output);
}

void counting_sort(std::vector<int64_t>& data) {
    if (data.empty()) return;
    auto max_val = *std::max_element(data.begin(), data.end());
    counting_sort(data, max_val);
}

} // namespace algorithms::sorting::counting_sort

