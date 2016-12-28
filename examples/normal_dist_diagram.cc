#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>

#include <chaos.h>

CHAOS_MACHINE_NCG gen;

int main(void) {
	std::normal_distribution<> d(5, 2);
	std::map<int, int> hist;

	for (int n = 0; n < 5 * 10000; ++n)
		++hist[std::round(d(gen))];

	for (auto p : hist)
		std::cout << std::fixed << std::setprecision(1) << std::setw(2) << p.first
		          << ' ' << std::string(p.second / 200, '*') << '\n';
}
