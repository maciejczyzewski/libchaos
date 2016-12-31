#include "chaos/engines/empty.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace engines { //::chaos::engines /////////////////////////////////////////

void empty::push(uint8_t block) {
	// what to do with 8-bit block in machine?
	(void)block;
}

uint8_t empty::pull(void) {
	// how-to pull 8-bit block from machine?
	return 0x0;
}

void empty::__reset(void) {
	// what should be cleared?
}

} //::chaos::engines ///////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////
