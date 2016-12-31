#ifndef CHAOS_GENERATORS_ABYSSINIAN_HH
#define CHAOS_GENERATORS_ABYSSINIAN_HH

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

// @1: typical next/round for abyssinian
#define R(x, y)                                                                \
	{                                                                            \
		x = (uint64_t)0xfffd21a7 * (uint32_t)x + (uint32_t)(x >> 32);              \
		y = (uint64_t)0xfffd1361 * (uint32_t)y + (uint32_t)(y >> 32);              \
	}

class abyssinian {
protected:
	static const uint64_t C1 = 0xff51afd7ed558ccdULL;
	static const uint64_t C2 = 0xc4ceb9fe1a85ec53ULL;

	uint64_t _x = C1, _y = C2;

public:
	// metadata
	CHAOS_META_DEFINE("abyssinian", "???");

	// typename
	typedef uint32_t size_seed;
	typedef uint32_t size_next;

	void seed(uint32_t x, uint32_t y) {
		x += y;
		y += x;

		uint64_t seed_x = 0x9368e53c2f6af274ULL ^ x;
		uint64_t seed_y = 0x586dcd208f7cd3fdULL ^ y;

		seed_x *= C1;
		seed_x ^= seed_x >> 33;
		seed_x *= C2;
		seed_x ^= seed_x >> 33;

		seed_y *= C1;
		seed_y ^= seed_y >> 33;
		seed_y *= C2;
		seed_y ^= seed_y >> 33;

		_x = seed_x;
		_y = seed_y;

		R(_x, _y);
	}

	void seed(uint32_t seed) { this->seed(seed, seed); }

	inline uint32_t next() {
		R(_x, _y);
		return CHAOS_RO_L32((uint32_t)_x, 7) + (uint32_t)_y;
	}
};

#undef R // @1

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_GENERATOR_REGISTER(ABYSSINIAN, chaos::generators::abyssinian);

#endif // CHAOS_GENERATORS_ABYSSINIAN_HH
