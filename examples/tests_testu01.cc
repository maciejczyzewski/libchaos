#include <chaos.h>
#include <iostream>

extern "C" {
#include "bbattery.h"
#include "smarsa.h"
#include "unif01.h"
#include "util.h"
}

// Chaos Machines

CHAOS_MACHINE_NCG x_0;
CHAOS_MACHINE_XORRING64 x_1;

// Pseudo-Random Number Generators

chaos::generators::xorshift<uint32_t, 1, 3, 10> x_2;
CHAOS_PRNG_ABYSSINIAN x_3;
chaos::generators::lcg<uint32_t, 48271, 0, 2147483647> x_4;
CHAOS_PRNG_KISS x_5;

// Adapters (double is universal in this case)

double UNIF01_NEXT_ADAPTER0() { return CHAOS_DOUBLE_U32(x_0.pull()); }
double UNIF01_NEXT_ADAPTER1() { return CHAOS_DOUBLE_U64(x_1.pull()); }
double UNIF01_NEXT_ADAPTER2() { return CHAOS_DOUBLE_U32(x_2.next()); }
double UNIF01_NEXT_ADAPTER3() { return CHAOS_DOUBLE_U32(x_3.next()); }
double UNIF01_NEXT_ADAPTER4() { return CHAOS_DOUBLE_U32(x_4.next()); }
double UNIF01_NEXT_ADAPTER5() { return CHAOS_DOUBLE_U32(x_5.next()); }

int main(void) {
	unif01_Gen *gen;
	gen = unif01_CreateExternGen01(CHAOS_META_NAME(CHAOS_MACHINE_XORRING64),
	                               UNIF01_NEXT_ADAPTER1);

	// smarsa_BirthdaySpacings(gen, NULL, 1, 1000, 0, 10000, 2, 1);
	// smarsa_BirthdaySpacings(gen, NULL, 1, 10000, 0, 1000000, 2, 1);

	// smarsa_BirthdaySpacings(gen, NULL, 3, 200000, 14, 256, 8, 1);
	// smarsa_BirthdaySpacings(gen, NULL, 3, 20000000, 14, 256, 8, 1);

	// bbattery_BlockAlphabit(gen, 1024 * 1024, 0, 8);
	// bbattery_Alphabit(gen, 1024 * 1024, 0, 8);

	// bbattery_FIPS_140_2(gen);
	// bbattery_Rabbit(gen, 10000000);
	bbattery_pseudoDIEHARD(gen);

	// bbattery_SmallCrush(gen);
	// bbattery_Crush(gen);
	// bbattery_BigCrush(gen);

	unif01_DeleteExternGenBits(gen);
}
