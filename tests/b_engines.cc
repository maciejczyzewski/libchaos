#include "benchmark/benchmark.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include <chaos.h>

/// machines ///////////////////////////////////////////////////////////////////

#define B_MACHINE(x)                                                           \
	x machine_##x;                                                               \
                                                                               \
	static void B__##x##__Push(benchmark::State &state) {                        \
		machine_##x.set_space(state.range(1));                                     \
		machine_##x.set_time(state.range(2));                                      \
                                                                               \
		while (state.KeepRunning()) {                                              \
			for (int i = state.range(0); i--;) {                                     \
				machine_##x.push(0xAA);                                                \
				machine_##x.push(0xBB);                                                \
				machine_##x.push(0xCC);                                                \
				machine_##x.push(0xDD);                                                \
			}                                                                        \
		}                                                                          \
                                                                               \
		state.SetComplexityN(state.range(0));                                      \
		state.SetItemsProcessed(state.iterations() * state.range(0));              \
		state.SetBytesProcessed(state.iterations() * state.range(0) *              \
		                        sizeof(x::size_push) * 4);                         \
	}                                                                            \
                                                                               \
	BENCHMARK(B__##x##__Push)                                                    \
	    ->RangeMultiplier(8)                                                     \
	    ->Ranges({{1 << 12, 1 << 12}, {1, 32}, {1, 32}})                         \
	    ->Complexity(benchmark::oAuto)                                           \
	    ->Unit(benchmark::kMicrosecond);                                         \
                                                                               \
	static void B__##x##__Pull(benchmark::State &state) {                        \
		machine_##x.set_space(state.range(1));                                     \
		machine_##x.set_time(state.range(2));                                      \
                                                                               \
		while (state.KeepRunning()) {                                              \
			for (int i = state.range(0); i--;) {                                     \
				machine_##x.pull();                                                    \
				machine_##x.pull();                                                    \
				machine_##x.pull();                                                    \
				machine_##x.pull();                                                    \
			}                                                                        \
		}                                                                          \
                                                                               \
		state.SetComplexityN(state.range(0));                                      \
		state.SetItemsProcessed(state.iterations() * state.range(0));              \
		state.SetBytesProcessed(state.iterations() * state.range(0) *              \
		                        sizeof(x::size_pull) * 4);                         \
	}                                                                            \
                                                                               \
	BENCHMARK(B__##x##__Pull)                                                    \
	    ->RangeMultiplier(8)                                                     \
	    ->Ranges({{1 << 12, 1 << 12}, {1, 32}, {1, 32}})                         \
	    ->Complexity(benchmark::oAuto)                                           \
	    ->Unit(benchmark::kMicrosecond);

B_MACHINE(CHAOS_MACHINE_NCG)
B_MACHINE(CHAOS_MACHINE_XORRING32)
B_MACHINE(CHAOS_MACHINE_XORRING64)
