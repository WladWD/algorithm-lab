#include <gtest/gtest.h>
#include "algebra/mod_inv/mod_inv.h"

using namespace algebra::mod_inv;

TEST(ModInv_Extended, BasicInvertible) {
    auto inv = modinv_ext(3, 11);
    ASSERT_TRUE(inv.has_value());
    EXPECT_EQ(*inv, 4);
}

TEST(ModInv_Extended, NegativeInput) {
    auto inv = modinv_ext(-3, 11);
    ASSERT_TRUE(inv.has_value());
    // -3 mod 11 == 8, inverse of 8 is 7 because 8*7 = 56 ≡ 1 (mod 11)
    EXPECT_EQ(*inv, 7);
}

TEST(ModInv_Extended, NonInvertible) {
    auto inv = modinv_ext(2, 6);
    EXPECT_FALSE(inv.has_value());
}

TEST(ModInv_Extended, LargeModulusProperty) {
    long long a = 123456789;
    long long mod = 1000000007; // prime
    auto inv = modinv_ext(a, mod);
    ASSERT_TRUE(inv.has_value());
    long long prod = (long long)((__int128)a * (*inv) % mod);
    EXPECT_EQ(prod, 1);
}

TEST(ModInv_Fermat, BasicInvertiblePrime) {
    long long a = 3, p = 11;
    long long inv = modinv_fermat(a, p);
    EXPECT_EQ(inv, 4);
}

TEST(ModInv_Fermat, LargePrime) {
    long long a = 123456789;
    long long p = 1000000007; // prime
    long long inv = modinv_fermat(a, p);
    long long prod = (long long)((__int128)a * inv % p);
    EXPECT_EQ(prod, 1);
}

TEST(ModInv_Fermat, InvalidPrecondition) {
    // Fermat's method requires p prime and a % p != 0. We test behavior for a=0 (should produce 0)
    long long a = 0, p = 1000000007;
    long long inv = modinv_fermat(a, p);
    // convention: 0^(p-2) == 0 for p>2, so result 0 and not invertible; this test documents behavior
    EXPECT_EQ(inv, 0);
}
