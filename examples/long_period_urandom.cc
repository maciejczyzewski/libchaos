#include <chaos.h> // library header
#include <iostream>

// initialize chaos machine (64-bit version)
CHAOS_MACHINE_XORRING64 machine;

int main(void) {
	machine.set_space(100000);     // 2^6400000 period length
	machine << 0x8a5cd789635d2dff; // initialize with seed
	while (true)
		putc_unlocked(machine.pull(), stdout);
}
