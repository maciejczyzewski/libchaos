#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <chaos.h>

CHAOS_MACHINE_XORRING64 gen;

void READ(const char *filename) {
	std::ifstream ifs(filename);
	std::vector<CHAOS_MACHINE_XORRING64::size_push> vec;

	vec.assign((std::istreambuf_iterator<char>(ifs)),
	           (std::istreambuf_iterator<char>()));

	for (auto block : vec)
		gen.push(block);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s [filename]\n", argv[0]);
		return 1;
	}

	gen.set_time(4);
	gen.set_space(1000); // signed with secret key
	gen.set_key({0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
	             0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
	             0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
	             0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
	             0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca});

	READ(argv[1]);

	for (size_t i = 0; i < 5 * 3; i += 3)
		printf("%p\t%p\t%p\n", gen.pull(), gen.pull(), gen.pull());

	return 0;
}
