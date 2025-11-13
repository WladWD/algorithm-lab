#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

namespace algebra::powmod {
uint64_t mul_mod_u128(uint64_t a, uint64_t b, uint64_t mod);

uint64_t powmod_u128(uint64_t base, uint64_t exp, uint64_t mod);

// powmod with decimal-string exponent. exp_dec must contain only digits and no sign.
uint64_t powmod_bigexp(uint64_t base, const std::string &exp_dec, uint64_t mod);
} // namespace algebra::powmod


