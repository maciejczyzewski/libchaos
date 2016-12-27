#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <chaos.h>

using namespace std;

CHAOS_MACHINE_NCG mc;

uint32_t checksum(std::ifstream &file) {
	CHAOS_MACHINE_NCG::size_push word;
	mc.reset();
	while (file.read(reinterpret_cast<char *>(&word), sizeof(word)))
		mc.push(word);
	mc.push(word);
}

int main(int argc, char **argv) {
	mc.set_space(2);
	mc.set_time(1);

	ifstream file(argv[1]);
	checksum(file); // FIXME: just for testing...

	for (size_t i = 0; i < 32; i++)
		printf("%p\t%p\t%p\t%p\n", mc.pull(), mc.pull(), mc.pull(), mc.pull());
}
