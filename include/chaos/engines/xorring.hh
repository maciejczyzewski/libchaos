#ifndef CHAOS_ENGINES_XORRING_HH
#define CHAOS_ENGINES_XORRING_HH

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

#include "chaos/machine.hh"
#include "chaos/macros.hh"
#include "chaos/seed.hh"
#include "chaos/types.hh"

#include "chaos/generators/xorshift.hh"
#include "chaos/prng.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace engines { //::chaos::engines /////////////////////////////////////////

class xorring32 {
public:
	// metadata
	CHAOS_META_DEFINE("xorring32", "Maciej A. Czyzewski");

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
	const size_t __default_space = 5, __default_time = 1;
	const std::vector<size_cell> __default_key = {
	    0xbbe112c5, 0xa8354e67, 0x22c32ce1, 0xe5980656, 0xda498845};

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
	std::deque<size_cell> buffer;
};

class xorring64 {
public:
	// metadata
	CHAOS_META_DEFINE("xorring64", "Maciej A. Czyzewski");

	// typename
	typedef uint64_t size_cell;
	typedef uint64_t size_push;
	typedef uint64_t size_pull;

	// interface
	void push(size_push);
	size_pull pull(void);

	// cleaning
	void __reset(void);

	// defaults
	const size_t __default_space = 5, __default_time = 1;
	const std::vector<size_cell> __default_key = {
	    0x75a3d46301ec2524, 0x4d4e9485ccf00d9c, 0x29649ef6dbdd5078,
	    0x13f10a775c5fb54f, 0xea41d5a3bb1545bb};

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
	std::deque<size_cell> buffer;
};

} //::chaos::engines ///////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////

CHAOS_ENGINE_REGISTER(XORRING32, chaos::engines::xorring32);
CHAOS_ENGINE_REGISTER(XORRING64, chaos::engines::xorring64);

#endif // CHAOS_ENGINES_XORRING_HH
