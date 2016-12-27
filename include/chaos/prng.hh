#ifndef CHAOS_PRNG_HH
#define CHAOS_PRNG_HH

#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////

template <class generator> class prng : public virtual generator {
public:
	// API
	constexpr static typename generator::size_next max(void) {
		return std::numeric_limits<typename generator::size_next>::max();
	}
	constexpr static typename generator::size_next min(void) {
		return std::numeric_limits<typename generator::size_next>::lowest();
	}
	friend prng &operator<<(prng &instance, // FIXME: with multi-blocks
	                        const typename generator::size_seed &block) {
		instance.seed(block);
		return instance;
	}
	friend prng &operator>>(prng &instance,
	                        typename generator::size_next &block) {
		block = instance.next();
		return instance;
	}
	inline typename generator::size_next operator()(void) noexcept {
		return this->next();
	}
};

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_PRNG_HH
