#ifndef CHAOS_SEED_HH
#define CHAOS_SEED_HH

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace chaos { //::chaos ////////////////////////////////////////////////////

template <class size_cell>
std::vector<size_cell> triple_mask(std::vector<size_cell> buffer,
                                   std::vector<size_cell> key,
                                   std::vector<size_cell> mask) {
	std::vector<size_cell> seeded(mask.size());
	if (buffer.size() == mask.size())
		seeded = buffer;

	if (buffer.size() > mask.size())
		buffer.resize(mask.size()), seeded = buffer;

	if (buffer.size() < mask.size()) {
		std::copy(buffer.begin(), buffer.end(), seeded.begin());
		std::copy(mask.begin() + buffer.size(), mask.end(),
		          seeded.begin() + buffer.size());
	}

	if (key.size() != 0) {
		std::copy(key.begin(), key.end(), seeded.begin());
	}

	return seeded;
}

template <class algorithm, size_t time, size_t space, size_t length>
std::string password(std::string pass, std::string salt) {
	algorithm gen;

	std::vector<typename algorithm::size_cell> key(salt.begin(), salt.end());
	std::vector<typename algorithm::size_push> blc(pass.begin(), pass.end());

	gen.set_key(key);
	gen.set_time(time);
	gen.set_space(space);

	for (auto block : blc)
		gen.push(block);

	std::ostringstream sign;
	sign << std::hex << std::uppercase << std::setfill('0');
	std::vector<typename algorithm::size_pull> vec(length, 0);

	std::generate(vec.begin(), vec.end(), [&gen] { return gen.pull(); });

	for (int block : vec)
		sign << std::setw(2) << block;

	return (sign.str()).substr(0, length);
}

template <class size_cell>
std::vector<size_cell> keystream(std::vector<size_cell> key, size_t length) {
	std::vector<size_cell> vec(length, 0);

	// FIXME: make it as algorithm (below my draft -- M.A.C.)
	for (size_t i = 0; i < key.size(); i++) // @1
		key[i] = key[i] * 0x586dcd208f7cd3fdULL +
		         key[key[i] % key.size()] * 0x9368e53c2f6af274ULL;
	vec[0] = key[0] + key[key.size() - 1]; // @2
	for (size_t i = 1; i < length; i++) // @3
		vec[i] = vec[i - 1] * 0x9368e53c2f6af274ULL +
		         key[vec[i - 1] % key.size()] * 0x586dcd208f7cd3fdULL;

	return vec;
}

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_SEED_HH
