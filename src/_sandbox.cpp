#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include <chaos.h>

int main() {
	std::random_device rd;

	CHAOS_MACHINE_NCG gen;
	CHAOS_PRNG_KISS prng;

	prng << 0x55;
	printf("\n");

	chaos::truely<CHAOS_MACHINE_NCG, std::random_device> x(100);

	for (size_t i = 0; i < 5000; i++)
		printf(">> %p\n", x());

	printf("SEED: %p\n", rd());

	gen << 0x44;
	gen.push(0x55);

	uint32_t a = 0x66;

	gen << a << 0x55;
	gen.push(a);

	uint32_t b, c;
	gen >> b >> c;

	printf("--> %p\n", b);
	printf("--> %p\n", c);
	printf("--> %p\n", gen());
	printf("--> %p\n", gen.pull());

	std::normal_distribution<> d(5, 2);

	std::map<int, int> hist;

	for (int n = 0; n < 100000; ++n)
		++hist[std::round(d(x))];

	for (auto p : hist)
		std::cout << std::fixed << std::setprecision(1) << std::setw(2) << p.first
		          << ' ' << std::string(p.second / 200, '*') << '\n';
}
