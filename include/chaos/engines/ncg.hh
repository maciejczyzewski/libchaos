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
	CHAOS_META_DEFINE("NCG (Naive Czyzewski Generator)", "Maciej A. Czyzewski");

	// typename
	typedef uint32_t size_cell;
	typedef uint32_t size_push;
	typedef uint32_t size_pull;

	// interface
	void push(size_push);
	size_pull pull(void);

	// cleaning
	void __reset(void);

	// defaults
	const size_t __default_space = 8, __default_time = 1;
	const std::vector<size_cell> __default_key = {
	    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501};

protected:
	// costs
	size_t __cost_space = 0, __cost_time = 0;

	// methods
	virtual void __set_key(std::vector<size_cell> value, size_t begin = 0) {
		// set starting variable
		for (std::vector<size_cell>::size_type i = begin, j = 0; j != value.size();
		     i++, j++) {
			// in NCG one hybrid system is build upon two cells in buffer
			this->buffer[i * 2] = value[j] >> 16;
			this->buffer[i * 2 + 1] = value[j] & 0xFFFF;
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
