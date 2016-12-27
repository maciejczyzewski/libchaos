#ifndef CHAOS_TRUELY_HH
#define CHAOS_TRUELY_HH

#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <limits>

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////

template <class machine, class device> class truely {
private:
	// modules
	machine local_cm;
	device local_rd;

	// reseeding
	size_t current_checkpoint;
	size_t current_position;

	// settings
	size_t skip_blocks;

public:
	// mechanism
	truely(size_t skip = 1000) {
		this->skip_blocks = skip;
		this->local_cm.push(this->local_rd());
		// generate checkpoint
		this->reseed();
	}
	inline void reseed(void) {
		if (this->current_position == this->current_checkpoint) {
			this->local_cm.push(this->local_rd());
			// save new checkpoint
			this->current_checkpoint =
			    (this->local_cm.pull() % this->skip_blocks) + 1;
			this->current_position = this->local_cm.pull() % this->current_checkpoint;
		}
	}

	// API
	constexpr static typename machine::size_pull max(void) {
		return std::numeric_limits<typename machine::size_pull>::max();
	}
	constexpr static typename machine::size_pull min(void) {
		return std::numeric_limits<typename machine::size_pull>::lowest();
	}
	inline typename machine::size_pull operator()(void) noexcept {
		this->reseed();
		this->current_position++;
		// generate number
		return this->local_cm.pull();
	}
};

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_TRUELY_HH
