#include "gtest/gtest.h"

#include <iostream>
#include <vector>

#include <chaos.h>

/// prng ///////////////////////////////////////////////////////////////////////

TEST(PRNG, T__CHAOS_PRNG__AdapterInitialization) {
	CHAOS_PRNG_KISS gen_1;

	for (size_t i = 0; i < 1000; i++)
		gen_1.next();
	gen_1 << 0x112233;
	for (size_t i = 0; i < 1000; i++)
		gen_1.next();

	CHAOS_PRNG_KISS gen_2;

	for (size_t i = 0; i < 1000; i++)
		gen_2.next();
	gen_2 << 0x112233;
	for (size_t i = 0; i < 1000; i++)
		gen_2.next();

	EXPECT_EQ(gen_1.next(), gen_2.next());
}

TEST(PRNG, T__CHAOS_PRNG__AdapterAPI) {
	CHAOS_PRNG_KISS gen_1;
	CHAOS_PRNG_KISS gen_2;
	CHAOS_PRNG_KISS gen_3;
	CHAOS_PRNG_KISS gen_4;

	EXPECT_EQ(gen_1(), gen_2.next());

	uint32_t tmp;
	gen_3 >> tmp;

	EXPECT_EQ(tmp, gen_4.next());

	gen_1 << 0x11;
	gen_2.seed(0x11);

	EXPECT_EQ(gen_1(), gen_2());
}
