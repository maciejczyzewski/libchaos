#include <chaos.h>

////////////////////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

typedef std::linear_congruential_engine<std::uint_fast32_t, 48271, 0,
                                        2147483647>
    STL_MINSTD_RAND;
typedef std::mersenne_twister_engine<std::uint_fast32_t, 32, 624, 397, 31,
                                     0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680,
                                     15, 0xefc60000, 18, 1812433253>
    STL_MT19937;
typedef std::mersenne_twister_engine<
    std::uint_fast64_t, 64, 312, 156, 31, 0xb5026f5aa96619e9, 29,
    0x5555555555555555, 17, 0x71d67fffeda60000, 37, 0xfff7eee000000000, 43,
    6364136223846793005>
    STL_MT19937_64;
typedef std::discard_block_engine<std::ranlux24_base, 223, 23> STL_RANLUX24;
typedef std::discard_block_engine<std::ranlux48_base, 389, 11> STL_RANLUX48;
typedef std::shuffle_order_engine<std::minstd_rand0, 256> STL_KNUTH_B;

#define CLOCK_POINT(a)                               \
  std::chrono::high_resolution_clock::time_point a = \
      std::chrono::high_resolution_clock::now();
#define CLOCK_DIFF(a, b) \
  std::chrono::duration_cast<std::chrono::nanoseconds>(b - a).count()

////////////////////////////////////////////////////////////////////////////////

#define BENCHMARK(algorithm, func, size_output)                      \
  {                                                                  \
    std::string name = #algorithm;                                   \
    printf("[%s]\n", name.c_str());                                  \
                                                                     \
    std::ofstream file;                                              \
    file.open(name + ".csv");                                        \
    file << "bytes,rate\n";                                          \
                                                                     \
    size_t d = 400;                                                  \
    if (sizeof(size_output) != 8) d *= 8 / (int)sizeof(size_output); \
                                                                     \
    for (size_t p = 1; p < d; p++) {                                 \
      fprintf(stderr, ".");                                          \
      size_t num = 5 * p;                                            \
      size_t rep = 500 * 1.5;                                        \
                                                                     \
      std::vector<uint64_t> v(rep);                                  \
                                                                     \
      for (size_t i = 0; i < rep; i++) {                             \
        uint64_t x = 0;                                              \
        CLOCK_POINT(A)                                               \
        for (size_t j = 0; j < num; j++) x ^= func;                  \
        CLOCK_POINT(B)                                               \
        v[i] = CLOCK_DIFF(A, B);                                     \
      }                                                              \
                                                                     \
      std::sort(v.begin(), v.end());                                 \
      v.erase(v.begin(), v.begin() + rep / 5);                       \
      v.resize(rep - 2 * rep / 5);                                   \
                                                                     \
      double sum = std::accumulate(v.begin(), v.end(), 0.0);         \
      double mean = sum / v.size();                                  \
                                                                     \
      size_t bytes = sizeof(size_output) * num;                      \
      file << (uint64_t)bytes << "," << mean << "\n";                \
    }                                                                \
                                                                     \
    file.close();                                                    \
    printf("\n\n");                                                  \
  }

#define B_CM(algorithm)                                              \
  {                                                                  \
    algorithm gen;                                                   \
    BENCHMARK(algorithm, gen.pull(), typename algorithm::size_pull); \
  }

#define B_PRNG(algorithm)                                            \
  {                                                                  \
    algorithm gen;                                                   \
    BENCHMARK(algorithm, gen.next(), typename algorithm::size_next); \
  }

#define B_PRNG_BASE(algorithm, typename_size)   \
  {                                             \
    algorithm gen;                              \
    BENCHMARK(algorithm, gen(), typename_size); \
  }

////////////////////////////////////////////////////////////////////////////////

int main(void) {
  // CMs
  B_CM(CHAOS_MACHINE_NCG);
  B_CM(CHAOS_MACHINE_XORRING32);
  B_CM(CHAOS_MACHINE_XORRING64);

  // PRNGs
  B_PRNG(CHAOS_PRNG_KISS)
  B_PRNG(CHAOS_PRNG_ABYSSINIAN)
  B_PRNG(CHAOS_PRNG_XOROSHIRO128PLUS)
  B_PRNG(CHAOS_PRNG_XOROSHIRO1024STAR)
  B_PRNG(CHAOS_PRNG_XORSHF96)

  // STLs
  B_PRNG_BASE(STL_MINSTD_RAND, uint_fast32_t)
  B_PRNG_BASE(STL_MT19937, uint_fast32_t)
  B_PRNG_BASE(STL_MT19937_64, uint_fast64_t)
  B_PRNG_BASE(STL_RANLUX24, uint_fast32_t)
  B_PRNG_BASE(STL_RANLUX48, uint_fast64_t)
  B_PRNG_BASE(STL_KNUTH_B, uint_fast32_t)

  return 0;
}
