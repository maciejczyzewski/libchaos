#ifndef CHAOS_ENGINES_EMPTY_HH
#define CHAOS_ENGINES_EMPTY_HH

#include <iostream>
#include <vector>

#include "chaos/machine.hh"
#include "chaos/macros.hh"
#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace engines { //::chaos::engines /////////////////////////////////////////

class empty {
public:
	// metadata
	CHAOS_META_DEFINE("Empty (Blank Template)", "Libchaos Authors");

	// typename
	typedef uint8_t size_cell;
	typedef uint8_t size_push;
	typedef uint8_t size_pull;

	// interface
	void push(size_push);
	size_pull pull(void);

	// cleaning
	void __reset(void);

	// defaults
	const size_t __default_space = 4, __default_time = 2;
	const std::vector<size_cell> __default_key = {0x11, 0x22, 0x33, 0x44};

protected:
	// costs
	size_t __cost_space = 0, __cost_time = 0;

	// methods
	virtual void __set_key(std::vector<size_cell> value, size_t begin = 0) {
		// set starting variable
		std::copy(value.begin(), value.end(), this->buffer.begin() + begin);
	}
	virtual void __set_space(size_t value) {
		// set new space parameter
		this->__cost_space = value;
		// resize machine spaces if needed
		this->buffer.resize(this->__cost_space);
	}
	virtual void __set_time(size_t value) {
		// set new time parameter
		this->__cost_time = value;
	}

private:
	// variables
	std::vector<size_cell> buffer;
};

} //::chaos::engines ///////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_ENGINE_REGISTER(EMPTY, chaos::engines::empty);

#endif // CHAOS_ENGINES_EMPTY_HH
