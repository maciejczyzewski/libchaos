#ifndef CHAOS_GENERATORS_XORSHIFT_HH
#define CHAOS_GENERATORS_XORSHIFT_HH

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

template <class utype, utype a, utype b, utype c> class xorshift {
protected:
	utype _a, _b, _c, s;

public:
	// metadata
	CHAOS_META_DEFINE("xorshift", "George Marsaglia");

	// typename
	typedef utype size_seed;
	typedef utype size_next;

	explicit xorshift() : _a(a), _b(b), _c(c), s(2463534242) {}

	void seed(utype seed) { s = seed; }

	inline utype next() {
		s ^= (s << a);
		s ^= (s >> b);
		return (s ^= (s << c));
	}
};

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

// CHAOS_GENERATOR_REGISTER(XORSHIFT, chaos::generators::xorshift);

#endif // CHAOS_GENERATORS_XORSHIFT_HH
