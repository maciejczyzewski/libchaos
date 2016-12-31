#ifndef CHAOS_GENERATORS_KISS_HH
#define CHAOS_GENERATORS_KISS_HH

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

#define znew (z = 36969 * (z & 65535) + (z >> 16))
#define wnew (w = 18000 * (w & 65535) + (w >> 16))
#define MWC ((znew << 16) + wnew)
#define SHR3 (jsr ^= (jsr << 17), jsr ^= (jsr >> 13), jsr ^= (jsr << 5))
#define CONG (jcong = 69069 * jcong + 1234567)
#define KISS ((MWC ^ CONG) + SHR3)

class kiss {
protected:
	uint32_t w = 0x46500000, z = 0x90690000;
	uint32_t jsr = 16807, jcong = 48271;

public:
	// metadata
	CHAOS_META_DEFINE("KISS (Keep It Simple Stupid)", "G. Marsaglia and A. Zaman");

	// typename
	typedef uint32_t size_seed;
	typedef uint32_t size_next;

	void seed(uint32_t x, uint32_t y) {
		w = 0x46500000;
		z = 0x90690000;
		jsr = x;
		jcong = y;
	}

	void seed(uint32_t seed) { this->seed(seed, seed); }

	inline uint32_t next(void) { return KISS; }
};

#undef znew
#undef wnew
#undef MWC
#undef SHR3
#undef CONG
#undef KISS

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_GENERATOR_REGISTER(KISS, chaos::generators::kiss);

#endif // CHAOS_GENERATORS_KISS_HH
