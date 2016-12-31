#ifndef CHAOS_GENERATORS_LINEAR_HH
#define CHAOS_GENERATORS_LINEAR_HH

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace generators { //::chaos::generators ///////////////////////////////////

// http://courses.cse.tamu.edu/csce680/walker/lfsr_table.pdf

template <class utype, utype a, utype c, utype m> class lcg {
protected:
	utype _a, _c, _m, s;

public:
	// metadata
	CHAOS_META_DEFINE("LCG (Linear Congruential Generator)", "???");

	// typename
	typedef utype size_seed;
	typedef utype size_next;

	explicit lcg() : _a(a), _c(c), _m(m), s(2147483648) {}

	void seed(utype seed) { s = seed; }

	inline utype next() { return (s = (_a * s + _c) % _m); }
};

} //::chaos::generators ////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

// CHAOS_GENERATOR_REGISTER(LCG, chaos::generators::lcg);

#endif // CHAOS_GENERATORS_LINEAR_HH
