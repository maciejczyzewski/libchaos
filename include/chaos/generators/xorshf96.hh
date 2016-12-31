#ifndef CHAOS_GENERATORS_XORSHF96_HH
#define CHAOS_GENERATORS_XORSHF96_HH

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

class xorshf96 {
protected:
	static const uint32_t _x = 123456789, _y = 362436069, _z = 521288629;

	uint32_t x = _x, y = _y, z = _z;

public:
	// metadata
	CHAOS_META_DEFINE("xorshf96", "George Marsaglia");

	// typename
	typedef uint32_t size_seed;
	typedef uint32_t size_next;

	void seed(uint32_t seed_1, uint32_t seed_2, uint32_t seed_3) {
		x = seed_1;
		y = seed_2;
		z = seed_3;
	}

	void seed(uint32_t seed) { this->seed(seed - 1, seed + 1, seed * seed - 1); }

	inline uint32_t next(void) {
		uint32_t t;

		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

		t = x;
		x = y;
		y = z;

		return z = t ^ x ^ y;
	}
};

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_GENERATOR_REGISTER(XORSHF96, chaos::generators::xorshf96);

#endif // CHAOS_GENERATORS_XORSHF96_HH
