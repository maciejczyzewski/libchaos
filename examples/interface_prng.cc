#include <chaos.h>

CHAOS_PRNG_XOROSHIRO128PLUS prng;

int main(void) {
	uint64_t a = 0x84242f96eca9c41d, b = 0xa3c65b8776f96855, c;

	// SEED
	prng.seed(a);
	prng << b;

	// NEXT
	prng.next();
	c << prng;
	prng();
}
