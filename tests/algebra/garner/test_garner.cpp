#include "algebra/garner/garner.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <vector>

using namespace algebra::garner;

// brute-force CRT solver for testing (returns smallest non-negative x or throws if no solution)
static uint64_t brute_force_crt(const std::vector<uint64_t>& a, const std::vector<uint64_t>& m) {
    if (a.size() != m.size())
        throw std::invalid_argument("size mismatch");
    uint64_t x = 0;
    uint64_t step = 1;
    for (size_t i = 0; i < a.size(); ++i) {
        bool found = false;
        for (uint64_t t = 0; t < m[i]; ++t) {
            if ((x + t * step) % m[i] == a[i]) {
                x = x + t * step;
                step *= m[i];
                found = true;
                break;
            }
        }
        if (!found)
            throw std::runtime_error("no solution in brute_force_crt");
    }
    return x;
}

TEST(GarnerTest, SimpleExample) {
    std::vector<uint64_t> m = {3, 5, 7};
    std::vector<uint64_t> a = {2, 3, 2};
    auto c = garner_mixed(a, m);
    // expected mixed digits from example: [2,2,1]
    ASSERT_EQ(c.size(), 3u);
    EXPECT_EQ(c[0], 2u);
    EXPECT_EQ(c[1], 2u);
    EXPECT_EQ(c[2], 1u);
    uint64_t x = garner_mod(a, m, UINT64_MAX); // reconstruct modulo 2^64 wrap
    // brute-force expected 23
    uint64_t bf = brute_force_crt(a, m);
    EXPECT_EQ(bf, 23u);
    EXPECT_EQ((uint64_t)x, (uint64_t)bf);
}

TEST(GarnerTest, RandomSmallModuli) {
    // test random small coprime moduli sets and random residues
    std::vector<std::vector<uint64_t>> mod_sets = {
        {2, 3, 5}, {3, 7, 11}, {5, 7, 9}, {4, 9, 25}
        // note: some are not pairwise-coprime; will skip invalid ones
    };
    for (auto& ms : mod_sets) {
        size_t k = ms.size();
        // test some residues
        for (uint64_t r = 0; r < 5; ++r) {
            std::vector<uint64_t> a(k);
            bool ok = true;
            for (size_t i = 0; i < k; ++i) {
                a[i] = (r + i) % ms[i];
            }
            // check pairwise coprime
            for (size_t i = 0; i < k && ok; ++i)
                for (size_t j = i + 1; j < k; ++j)
                    if (std::gcd(ms[i], ms[j]) != 1)
                        ok = false;
            if (!ok)
                continue;
            auto c = garner_mixed(a, ms);
            uint64_t x = garner_mod(a, ms, UINT64_MAX);
            uint64_t bf = brute_force_crt(a, ms);
            EXPECT_EQ((uint64_t)x, bf);
            // reconstruct from mixed-radix to verify
            uint64_t recon = 0;
            uint64_t mult = 1;
            for (size_t i = 0; i < c.size(); ++i) {
                recon += c[i] * mult;
                mult *= ms[i];
            }
            EXPECT_EQ(recon % mult, bf % mult);
        }
    }
}

TEST(GarnerTest, NonCoprimeThrows) {
    std::vector<uint64_t> m = {6, 10}; // gcd = 2
    std::vector<uint64_t> a = {1, 3};
    // modinv used internally should throw when gcd != 1 during precompute
    EXPECT_THROW(garner_mixed(a, m), std::invalid_argument);
}
