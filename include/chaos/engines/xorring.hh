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

#define CHAOS_TRIPLE_MASK(size_cell, buffer, key, cost_space, prng)            \
	std::vector<size_cell> _buffer(buffer.begin(), buffer.end());                \
	std::vector<size_cell> mask(cost_space), _value;                             \
	std::generate(mask.begin(), mask.end(), [&prng] { return prng.next(); });    \
	_value = chaos::triple_mask<size_cell>(_buffer, key, mask);                  \
	buffer.resize(_value.size());                                                \
	std::copy(_value.begin(), _value.end(), buffer.begin());

class xorring32 {
public:
	// metadata
	static const std::string name, authors;

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
	virtual void __set_key(std::vector<size_cell> value) {
		// set starting variable
		chaos::prng<chaos::generators::xorshift<size_cell, 3, 13, 7>> prng;
		CHAOS_TRIPLE_MASK(size_cell, this->buffer, value, this->__cost_space, prng);
	}
	virtual void __set_space(size_t value) {
		// set new space parameter
		this->__cost_space = value;
		// resize machine spaces if needed
		chaos::prng<chaos::generators::xorshift<size_cell, 3, 13, 7>> prng;
		CHAOS_TRIPLE_MASK(size_cell, this->buffer, {}, this->__cost_space, prng);
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
	static const std::string name, authors;

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
	virtual void __set_key(std::vector<size_cell> value) {
		// set starting variable
		chaos::prng<chaos::generators::xorshift<size_cell, 4, 9, 13>> prng;
		CHAOS_TRIPLE_MASK(size_cell, this->buffer, value, this->__cost_space, prng);
	}
	virtual void __set_space(size_t value) {
		// set new space parameter
		this->__cost_space = value;
		// resize machine spaces if needed
		chaos::prng<chaos::generators::xorshift<size_cell, 4, 9, 13>> prng;
		CHAOS_TRIPLE_MASK(size_cell, this->buffer, {}, this->__cost_space, prng);
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
