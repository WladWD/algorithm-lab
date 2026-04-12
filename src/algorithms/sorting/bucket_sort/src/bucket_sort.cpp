#include "algorithms/sorting/bucket_sort/bucket_sort.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace algorithms::sorting::bucket_sort {

namespace {

template <typename T>
void insertion_sort_vec(std::vector<T>& v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        auto key = std::move(v[i]);
        std::size_t j = i;
        while (j > 0 && v[j - 1] > key) {
            v[j] = std::move(v[j - 1]);
            --j;
        }
        v[j] = std::move(key);
    }
}

} // namespace

void bucket_sort(std::vector<double>& data, std::size_t num_buckets) {
    if (data.size() <= 1) return;

    const std::size_t n = data.size();
    if (num_buckets == 0) num_buckets = n;

    // Find range.
    double min_val = data[0], max_val = data[0];
    for (auto v : data) {
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
    }

    double range = max_val - min_val;
    if (range == 0.0) return; // all elements equal

    // Small epsilon to ensure max_val falls in the last bucket, not out of bounds.
    range = std::nextafter(range, std::numeric_limits<double>::max());

    std::vector<std::vector<double>> buckets(num_buckets);
    for (auto v : data) {
        auto idx = static_cast<std::size_t>((v - min_val) / range * static_cast<double>(num_buckets));
        if (idx >= num_buckets) idx = num_buckets - 1;
        buckets[idx].push_back(v);
    }

    // Sort each bucket and concatenate.
    std::size_t pos = 0;
    for (auto& bucket : buckets) {
        insertion_sort_vec(bucket);
        for (auto v : bucket) {
            data[pos++] = v;
        }
    }
}

void bucket_sort_int(std::vector<int64_t>& data, std::size_t num_buckets) {
    if (data.size() <= 1) return;

    const std::size_t n = data.size();
    if (num_buckets == 0) num_buckets = n;

    int64_t min_val = data[0], max_val = data[0];
    for (auto v : data) {
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
    }

    int64_t range = max_val - min_val;
    if (range == 0) return;

    std::vector<std::vector<int64_t>> buckets(num_buckets);
    for (auto v : data) {
        auto idx = static_cast<std::size_t>(
            static_cast<double>(v - min_val) / static_cast<double>(range + 1) *
            static_cast<double>(num_buckets));
        if (idx >= num_buckets) idx = num_buckets - 1;
        buckets[idx].push_back(v);
    }

    std::size_t pos = 0;
    for (auto& bucket : buckets) {
        insertion_sort_vec(bucket);
        for (auto v : bucket) {
            data[pos++] = v;
        }
    }
}

} // namespace algorithms::sorting::bucket_sort

