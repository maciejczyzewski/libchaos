#ifndef CHAOS_ENGINES_NCG_HH
#define CHAOS_ENGINES_NCG_HH

#include <iostream>
#include <vector>

#include "chaos/machine.hh"
#include "chaos/macros.hh"
#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace engines { //::chaos::engines /////////////////////////////////////////

class ncg {
public:
	// metadata
	static const std::string name, authors;

	// typename
	typedef uint8_t size_cell;
	typedef uint32_t size_push;
	typedef uint32_t size_pull;

	// interface
	void push(size_push);
	size_pull pull(void);

	// cleaning
	void __reset(void);

	// defaults
	const size_t __default_space = 8, __default_time = 1;
	const std::vector<size_cell> __default_key = {0x14, 0x15, 0x92, 0x65,
	                                              0x35, 0x89, 0x79, 0x32};

protected:
	// costs
	size_t __cost_space = 0, __cost_time = 0;

	// methods
	virtual void __set_key(std::vector<size_cell> value) {
		// resize key if needed
		value.resize(this->__cost_space);
		// set starting variable
		for (std::vector<size_cell>::size_type i = 0; i != value.size(); i++) {
			// in NCG one hybrid system is build upon two cells in buffer
			this->buffer[i * 2] = (value[i] >> 4) & 0x0F;
			this->buffer[i * 2 + 1] = (value[i] >> 0) & 0x0F;
		}
	}
	virtual void __set_space(size_t value) {
		// set new space parameter
		this->__cost_space = value;
		// resize machine spaces if needed
		this->buffer.resize(this->__cost_space * 2);
	}
	virtual void __set_time(size_t value) {
		// set new time parameter
		this->__cost_time = value;
	}

private:
	// spaces in algorithm
	std::vector<uint16_t> buffer;
	uint16_t params[2] = {0};
	// S - seed, I - increment
	uint32_t S, I;
};

} //::chaos::engines ///////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_ENGINE_REGISTER(NCG, chaos::engines::ncg);

#endif // CHAOS_ENGINES_NCG_HH
