#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string>

#include "chaos/analysis.hh"
#include "chaos/macros.hh"
#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////

double arithmetic_average(AP adapter) {
	int64_t a = 1000000 / 2;
	double s = 0;

	for (int64_t i = 0; i < a; i++)
		s += adapter();

	double var = s / a;

	double min_exp = 0.5 - 0.005;
	double max_exp = 0.5 + 0.005;

	double p_value = ((100 * (var - min_exp)) / (max_exp - min_exp)) / 100;

	return p_value;
}

double dirichlet_probe(AP adapter) {
	int64_t a = 10000000 / 2;
	int64_t b = 10;

	std::vector<int64_t> s(b, 0);

	for (int64_t i = 0; i < a; i++)
		s[(int64_t)(adapter() * b)]++;

	int64_t min_e = *std::min_element(s.begin(), s.end());
	int64_t max_e = *std::max_element(s.begin(), s.end());

	double var = max_e - min_e;
	double exp = 2500;

	return var / exp;
}

double pi_calculus(AP adapter) {
	int64_t points = 100000 / 2;

	int64_t circle_area = 0;
	int64_t square_area = 0;

	for (int64_t i = 0; i < points; i++) {
		double x = adapter();
		double y = adapter();

		if ((x * x + y * y < 1) == 1)
			circle_area++;

		square_area++;
	}

	double min_exp = 3.1415 - 0.002;
	double max_exp = 3.1415 + 0.002;

	double var = 4.0 * circle_area / square_area;

	/*
	[min,max] --> [a,b]

	       (b-a)(x - min)
	f(x) = --------------  + a
	          max - min
	*/

	double p_value = ((100 * (var - min_exp)) / (max_exp - min_exp)) / 100;

	return p_value;
}

} //::chaos ////////////////////////////////////////////////////////////////////
