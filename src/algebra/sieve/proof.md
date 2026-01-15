# Correctness proof — Sieve of Eratosthenes

We prove that the algorithm implemented in `is_prime_up_to(n)` correctly marks each integer `k` with `2 <= k <= n` as prime iff it is prime.

## Definitions
- A number `k >= 2` is **prime** if its only positive divisors are `1` and `k`.
- A number `k >= 2` is **composite** if it is not prime.

## Algorithm recap
Initialize `is_prime[i] = true` for all `i`, then set `is_prime[0] = is_prime[1] = false`.
For each integer `p` from `2` while `p*p <= n`:
- if `is_prime[p]` is true, mark all multiples `x = p*p, p*p+p, p*p+2p, ... <= n` as composite (set `is_prime[x] = false`).

## Lemma 1: Every number marked composite is composite
We only mark numbers of the form `x = p * m` where `p >= 2` and `m >= p` (because `x` starts at `p*p`).
Such an `x` has non-trivial divisors `p` and `m`, so `x` is composite.

## Lemma 2: Every composite number up to `n` is eventually marked
Let `k` be composite with `2 <= k <= n`.
Then `k` has a prime divisor `p` with `2 <= p <= sqrt(k)`.
In particular, `p <= sqrt(k) <= sqrt(n)`.

When the outer loop reaches this `p`, `is_prime[p]` is still true because no smaller factor can mark a prime as composite.
Now write `k = p * m`. Since `p <= sqrt(k)`, we have `m >= p`, hence `k >= p*p`.
Therefore `k` appears in the inner loop sequence starting at `p*p` and will be marked composite.

## Theorem: After termination, `is_prime[k]` is true iff `k` is prime
- By Lemma 1, all `false` entries correspond to composite numbers (plus 0 and 1).
- By Lemma 2, every composite `k` is set to `false` by the time the algorithm finishes.

Thus the remaining `true` entries for `k >= 2` are exactly the primes.

