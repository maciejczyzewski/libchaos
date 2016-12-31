#ifndef CHAOS_GENERATORS_XOROSHIRO_HH
#define CHAOS_GENERATORS_XOROSHIRO_HH

#include <cstring>

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

// @1: default memory alloc.
#define M(x, y, z)                                                             \
	{ std::memcpy(&x, &y, z * sizeof(uint64_t)); }

class xoroshiro128plus {
protected:
	uint64_t s[2], c[2] = {0x8a5cd789635d2dff, 0x121fd2155c472f96};

public:
	// metadata
	CHAOS_META_DEFINE("xoroshiro128+", "David Blackman and Sebastiano Vigna");

	// typename
	typedef uint64_t size_seed;
	typedef uint64_t size_next;

	explicit xoroshiro128plus(void) { M(s, c, 2); };

	void seed(uint64_t seed) {
		M(s, c, 2);
		s[0] = seed;
	}

	inline uint64_t next(void) {
		uint64_t s1 = s[0];
		const uint64_t s0 = s[1];
		const uint64_t result = s0 + s1;
		s[0] = s0;
		s1 ^= s1 << 23;                          // a
		s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
		return result;
	}
};

////////////////////////////////////////////////////////////////////////////////

class xoroshiro1024star {
protected:
	uint64_t s[16],
	    c[16] = {0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
	             0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
	             0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
	             0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
	             0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca,
	             0x284600e3f30e38c3};
	uint32_t p = 0;

public:
	// metadata
	CHAOS_META_DEFINE("xoroshiro1024*", "David Blackman and Sebastiano Vigna");

	// typename
	typedef uint64_t size_seed;
	typedef uint64_t size_next;

	explicit xoroshiro1024star(void) { M(s, c, 16); };

	void seed(uint64_t seed) {
		M(s, c, 16);
		s[p = 0] = seed;
	}

	inline uint64_t next(void) {
		const uint64_t s0 = s[p];
		uint64_t s1 = s[p = (p + 1) & 15];
		s1 ^= s1 << 31;                           // a
		s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
		return s[p] * UINT64_C(1181783497276652981);
	}
};

#undef M // @1

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_GENERATOR_REGISTER(XOROSHIRO128PLUS, chaos::generators::xoroshiro128plus);
CHAOS_GENERATOR_REGISTER(XOROSHIRO1024STAR,
                         chaos::generators::xoroshiro1024star);

#endif // CHAOS_GENERATORS_XOROSHIRO_HH
