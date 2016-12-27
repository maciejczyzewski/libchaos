#include "gtest/gtest.h"

#include <iostream>
#include <random>
#include <vector>

#include <chaos.h>

/// mixins /////////////////////////////////////////////////////////////////////

TEST(MIXINS, T__CHAOS_TRUELY__AdapterInitialization) {
	chaos::truely<CHAOS_MACHINE_NCG, std::random_device> gen_0, gen_1;

	size_t n = 100;
	std::vector<CHAOS_MACHINE_NCG::size_pull> a(n), b(n);

	for (size_t i = 0; i < n; i++)
		a.push_back(gen_0());

	for (size_t i = 0; i < n; i++)
		b.push_back(gen_1());

	EXPECT_NE(a, b);
}

/* FIXME
TEST(MIXINS, MACROS) {
	// machines
	EXPECT_EQ(CHAOS_META_NAME(CHAOS_MACHINE_NCG),
	          "NCG (Naive Czyzewski Generator)");
	EXPECT_EQ(CHAOS_META_AUTHORS(CHAOS_MACHINE_NCG), "Maciej A. Czyzewski");

	// prngs
	EXPECT_EQ(CHAOS_META_NAME(CHAOS_PRNG_KISS), "KISS (Keep It Simple Stupid)");
	EXPECT_EQ(CHAOS_META_AUTHORS(CHAOS_PRNG_KISS), "G. Marsaglia and A. Zaman");
}
*/
