/*  NCG written in 2015 by Maciej A. Czyzewski
To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.
See <http://creativecommons.org/publicdomain/zero/1.0/>.  */

#include "chaos/engines/ncg.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////
namespace engines { //::chaos::engines /////////////////////////////////////////

// @1: abbreviation for getting values from the tape
#define M(i) ((i) % (this->__cost_space * 2))

// @2: bits rotation formula (with untypical shift)
#define R(n, r)                                                                \
	((uint32_t)((uint32_t)(n) << (r)) | (uint32_t)((uint32_t)(n) >> (16 - (r))))

void ncg::push(uint32_t block) {
	// preparation
	const uint32_t S0 = block + 1, S1 = block - 1;
	S = block, I = block * 0x3C6EF35F;

	for (size_t i = 0; i < this->__cost_space * 2; i += 2) {
		// reinforcement & finalization [@1]
		const uint16_t b0 =
		    buffer[M(i + 0)] ^ (((I * S0) ^ S) >> 16) ^ ((I * S1) ^ S);
		buffer[M(i + 0)] = b0,
		             I ^= ((buffer[M(I - 1)] + b0) << 16) ^ (buffer[M(I + 1)] - b0);

		// reinforcement & finalization [@2]
		const uint16_t b1 =
		    buffer[M(i + 1)] ^ (((I * S0) ^ S) >> 16) ^ ((I * S1) ^ S);
		buffer[M(i + 1)] = b1,
		             I ^= ((buffer[M(I - 1)] + b1) << 16) ^ (buffer[M(I + 1)] - b1);
	}
}

uint32_t ncg::pull(void) {
	// variables
	const uint16_t a = buffer[M(I + 0)], b = buffer[M(I + 1)];

	// initialization
	uint32_t X = (a + I) * (b - S);

	// chaos
	const uint16_t Y =
	    (buffer[M(X - b)] << (a % 9)) ^ (buffer[M(X + a)] >> (b % 9));

	// rounds
	for (size_t i = 0; i < this->__cost_time * 2; i += 2) {
		const uint16_t b0 = buffer[M(I + i - 2)], b1 = buffer[M(I + i + 2)];

		// absorption
		params[0] ^= b0, params[1] ^= b1;

		// mixing + modification
		params[0] ^= (params[1] ^= R(Y, params[0] % 17));
		buffer[M(I + i - 2)] = b0 - (params[1] += (X & params[0]));
		params[1] += (params[0] += R(X, params[1] % 17));
		buffer[M(I + i + 2)] = b1 + (params[0] += (Y & params[1]));
	}

	// transformation
	buffer[M(I + 0)] = // chaotic map
	    R(params[0], X % 17) ^ R(params[1], X % 17) ^ (X & a) ^ (Y & b);
	buffer[M(I + 1)] = (b >> 1) ^ (-(b & 1u) & 0xB400u); // LFSR

	// finalization
	X += params[0] ^ (b << 8) ^ (params[1] << 16) ^ (a & 0xFF) ^ ((a >> 8) << 24);

	// cleaning
	params[0] = params[1] = 0xFFFF;

	// increment
	I += 2;

	return X;
}

void ncg::__reset(void) {
	// clear parameters space
	params[0] = params[1] = 0;
	S = 0x19660D00; // seed is not defined (prime)
	I = 0x3C6EF35F; // increment should be set
}

#undef M // @1
#undef R // @2

} //::chaos::engines ///////////////////////////////////////////////////////////
} //::chaos ////////////////////////////////////////////////////////////////////
