#ifndef CHAOS_ANALYSIS_HH
#define CHAOS_ANALYSIS_HH

#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>
#include <limits>

#include "chaos/types.hh"

namespace chaos { //::chaos ////////////////////////////////////////////////////

typedef double (*AP)(void);

////////////////////////////////////////////////////////////////////////////////

#define CHAOS_ANALYSIS_TEST(f, r, o)                                           \
	{                                                                            \
		for (size_t i = 0; i < r; i++)                                             \
			o += f;                                                                  \
		o /= r;                                                                    \
	}

#define CHAOS_ANALYSIS_RESULT(f, o)                                            \
	{                                                                            \
		if (o >= 1 || o <= 0) {                                                    \
			printf("%30s | %12f | \x1b[31mFAILED\x1b[0m\n", #f, o);                  \
		} else {                                                                   \
			printf("%30s | %12f | \x1b[32mPASSED\x1b[0m\n", #f, o);                  \
		}                                                                          \
	}

////////////////////////////////////////////////////////////////////////////////

class basic_adapter {
	AP adapter;

public:
	void connect(AP func) { adapter = func; }
	constexpr static size_t max(void) {
		return std::numeric_limits<uint32_t>::max();
	}
	constexpr static size_t min(void) {
		return std::numeric_limits<uint32_t>::lowest();
	}
	uint32_t operator()(void) noexcept {
		return (uint32_t)(adapter() * (double)UINT32_MAX);
	}
};

////////////////////////////////////////////////////////////////////////////////

double arithmetic_average(AP);
double dirichlet_probe(AP);
double pi_calculus(AP);

////////////////////////////////////////////////////////////////////////////////

template <typename TDistribution>
double sample_mean(basic_adapter adapter, TDistribution dist, int sample_size) {
	double sum = 0;
	for (int i = 0; i < sample_size; ++i)
		sum += dist(adapter);
	return sum / sample_size;
}

template <typename TDistribution>
double test_mean(basic_adapter adapter, TDistribution dist, double true_mean,
                 double true_variance) {
	double true_stdev = sqrt(true_variance);
	int sample_size = 10000000 / 50;
	double mean = sample_mean(adapter, dist, sample_size);
	double lower = true_mean - true_stdev / sqrt((double)sample_size);
	double upper = true_mean + true_stdev / sqrt((double)sample_size);
	return ((100 * (mean - lower)) / (upper - lower)) / 100;
}

////////////////////////////////////////////////////////////////////////////////

class analysis {
	AP adapter_01;
	basic_adapter adapter;

public:
	explicit analysis(AP x) {
		adapter_01 = x;
		adapter.connect(x);
	};
	void raport(void) {
		double result = 0;

		CHAOS_ANALYSIS_TEST(arithmetic_average(adapter_01), 100, result);
		CHAOS_ANALYSIS_RESULT(arithmetic_average, result);

		CHAOS_ANALYSIS_TEST(pi_calculus(adapter_01), 500, result);
		CHAOS_ANALYSIS_RESULT(pi_calculus, result);

		CHAOS_ANALYSIS_TEST(dirichlet_probe(adapter_01), 25, result);
		CHAOS_ANALYSIS_RESULT(dirichlet_probe, result);

		int n;
		double p, lambda, shape, mu, sigma;

		n = 5;
		p = 0.3;
		std::binomial_distribution<int> binomial(n, p);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, binomial, n * p, n * p * (1 - p)),
		                    50, result);
		CHAOS_ANALYSIS_RESULT(binomial_distribution, result);

		lambda = 4.0;
		std::exponential_distribution<double> exponential(lambda);
		CHAOS_ANALYSIS_TEST(
		    test_mean(adapter, exponential, 1.0 / lambda, 1.0 / (lambda * lambda)),
		    50, result);
		CHAOS_ANALYSIS_RESULT(exponential_distribution, result);

		shape = 3.0;
		std::gamma_distribution<double> gamma(shape);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, gamma, shape, shape), 50, result);
		CHAOS_ANALYSIS_RESULT(gamma_distribution, result);

		p = 0.001;
		std::geometric_distribution<int> geometric(p);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, geometric, 1.0 / p, 1.0 / (p * p)),
		                    50, result);
		CHAOS_ANALYSIS_RESULT(geometric_distribution, result);

		mu = 3.0;
		sigma = 4.0;
		std::normal_distribution<double> normal(mu, sigma);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, normal, mu, sigma * sigma), 50,
		                    result);
		CHAOS_ANALYSIS_RESULT(normal_distribution, result);

		lambda = 7.0;
		std::poisson_distribution<int> poisson(7.0);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, poisson, lambda, lambda), 50,
		                    result);
		CHAOS_ANALYSIS_RESULT(poisson_distribution, result);

		p = 0.6;
		std::bernoulli_distribution bernoulli(p);
		CHAOS_ANALYSIS_TEST(test_mean(adapter, bernoulli, p, p * (1 - p)), 50,
		                    result);
		CHAOS_ANALYSIS_RESULT(bernoulli_distribution, result);
	};
};

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_ANALYSIS_HH
