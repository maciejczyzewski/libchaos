#include "benchmark/benchmark.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include <chaos.h>

/// prng ///////////////////////////////////////////////////////////////////////

#define B_PRNG(x)                                                              \
	x prng_##x;                                                                  \
                                                                               \
	static void B__##x##__Next(benchmark::State &state) {                        \
		while (state.KeepRunning()) {                                              \
			for (int i = state.range(0); i--;) {                                     \
				prng_##x.next();                                                       \
				prng_##x.next();                                                       \
				prng_##x.next();                                                       \
				prng_##x.next();                                                       \
			}                                                                        \
		}                                                                          \
                                                                               \
		state.SetComplexityN(state.range(0));                                      \
		state.SetItemsProcessed(state.iterations() * state.range(0));              \
		state.SetBytesProcessed(state.iterations() * state.range(0) *              \
		                        sizeof(x::size_next) * 4);                         \
	}                                                                            \
                                                                               \
	BENCHMARK(B__##x##__Next)                                                    \
	    ->RangeMultiplier(4)                                                     \
	    ->Range(1 << 14, 1 << 20)                                                \
	    ->Complexity(benchmark::oAuto)                                           \
	    ->Unit(benchmark::kMicrosecond);

#define B_PRNG_BASE(x, y)                                                      \
	x prng_##x;                                                                  \
                                                                               \
	static void B__##x##__Next(benchmark::State &state) {                        \
		while (state.KeepRunning()) {                                              \
			for (int i = state.range(0); i--;) {                                     \
				prng_##x();                                                            \
				prng_##x();                                                            \
				prng_##x();                                                            \
				prng_##x();                                                            \
			}                                                                        \
		}                                                                          \
                                                                               \
		state.SetComplexityN(state.range(0));                                      \
		state.SetItemsProcessed(state.iterations() * state.range(0));              \
		state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(y) *  \
		                        4);                                                \
	}                                                                            \
                                                                               \
	BENCHMARK(B__##x##__Next)                                                    \
	    ->RangeMultiplier(4)                                                     \
	    ->Range(1 << 14, 1 << 20)                                                \
	    ->Complexity(benchmark::oAuto)                                           \
	    ->Unit(benchmark::kMicrosecond);

/// lib ///////////////////////////////////////////////////////////////////////

B_PRNG(CHAOS_PRNG_KISS)
B_PRNG(CHAOS_PRNG_ABYSSINIAN)
B_PRNG(CHAOS_PRNG_XOROSHIRO128PLUS)
B_PRNG(CHAOS_PRNG_XOROSHIRO1024STAR)
B_PRNG(CHAOS_PRNG_XORSHF96)

/// stl ////////////////////////////////////////////////////////////////////////

typedef std::linear_congruential_engine<std::uint_fast32_t, 48271, 0,
                                        2147483647>
    STL_MINSTD_RAND;
typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31,
                                     0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680,
                                     15, 0xefc60000, 18, 1812433253>
    STL_MT19937;
typedef std::mersenne_twister_engine<
    std::uint_fast64_t, 64, 312, 156, 31, 0xb5026f5aa96619e9, 29,
    0x5555555555555555, 17, 0x71d67fffeda60000, 37, 0xfff7eee000000000, 43,
    6364136223846793005>
    STL_MT19937_64;
typedef std::discard_block_engine<std::ranlux24_base, 223, 23> STL_RANLUX24;
typedef std::discard_block_engine<std::ranlux48_base, 389, 11> STL_RANLUX48;
typedef std::shuffle_order_engine<std::minstd_rand0, 256> STL_KNUTH_B;

B_PRNG_BASE(STL_MINSTD_RAND, uint_fast32_t)
B_PRNG_BASE(STL_MT19937, uint_fast32_t)
B_PRNG_BASE(STL_MT19937_64, uint_fast64_t)
B_PRNG_BASE(STL_RANLUX24, uint_fast32_t)
B_PRNG_BASE(STL_RANLUX48, uint_fast64_t)
B_PRNG_BASE(STL_KNUTH_B, uint_fast32_t)
