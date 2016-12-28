#include <chaos.h>
#include <iostream>

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
	std::cout << "=== XORRING ==================================================="
	          << std::endl;
	chaos::analysis gen1(UNIF01_NEXT_ADAPTER1);
	gen1.raport();
	std::cout << "=== XORSHIFT =================================================="
	          << std::endl;
	chaos::analysis gen2(UNIF01_NEXT_ADAPTER2);
	gen2.raport();
	std::cout << "=== ABYSSINIAN ================================================"
	          << std::endl;
	chaos::analysis gen3(UNIF01_NEXT_ADAPTER3);
	gen3.raport();
	std::cout << "=== KISS ======================================================"
	          << std::endl;
	chaos::analysis gen5(UNIF01_NEXT_ADAPTER5);
	gen5.raport();
	std::cout << "=== LCG ======================================================="
	          << std::endl;
	chaos::analysis gen4(UNIF01_NEXT_ADAPTER4);
	gen4.raport();
	std::cout << "=== NCG ======================================================="
	          << std::endl;
	chaos::analysis gen0(UNIF01_NEXT_ADAPTER0);
	gen0.raport();
}
