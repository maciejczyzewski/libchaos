#ifndef CHAOS_SEED_HH
#define CHAOS_SEED_HH

#include <iostream>
#include <stdexcept>
#include <vector>

namespace chaos { //::chaos ////////////////////////////////////////////////////

// change to triple_mask

template <class size_cell>
std::vector<size_cell> triple_mask(std::vector<size_cell> buffer,
                                   std::vector<size_cell> key,
                                   std::vector<size_cell> mask) {
	std::vector<size_cell> seeded(mask.size());
	/*
	  printf("\n------------------------\n");

	  for (size_t i = 0; i < buffer.size(); i++)
	    printf("[buffer] %p\n", buffer[i]);

	  for (size_t i = 0; i < key.size(); i++)
	    printf("[key   ] %p\n", key[i]);

	  for (size_t i = 0; i < mask.size(); i++)
	    printf("[mask  ] %p\n", mask[i]);

	  printf("SIZE: %d\n", mask.size());
	*/
	if (buffer.size() == mask.size())
		seeded = buffer;
	if (buffer.size() > mask.size()) {
		buffer.resize(mask.size());
		seeded = buffer;
	}
	if (buffer.size() < mask.size()) {
		std::copy(buffer.begin(), buffer.end(), seeded.begin());
		std::copy(mask.begin() + buffer.size(), mask.end(),
		          seeded.begin() + buffer.size());
	}

	if (key.size() != 0) {
		std::copy(key.begin(), key.end(), seeded.begin());
	}
	/*
	  for (size_t i = 0; i < seeded.size(); i++)
	    printf("[seeded] %p\n", seeded[i]);

	  printf("\n------------------------\n");
	*/
	return seeded;
}

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_SEED_HH
