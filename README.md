<a href="https://github.com/maciejczyzewski/libchaos">
    <img src="http://maciejczyzewski.me/libchaos/_static/libchaos.svg" alt="libchaos"
         title="libchaos" align="right" width="100" height="100" />
</a>
[![Build Status](https://travis-ci.org/maciejczyzewski/libchaos.svg?branch=master)](https://travis-ci.org/maciejczyzewski/libchaos)

[Overview](#overview) |
[Features](#features) |
[Installation](#installation) |
[Quick Start](#quick-start) |
[User Guide](#user-guide) |
[Contributing](#contributing) |
[License](#license)

**Got a question?** Join us on [stackoverflow](http://stackoverflow.com/questions/tagged/libchaos) or chat on [IRC](https://webchat.freenode.net/?channels=#libchaos).

*An advanced library for randomization, hashing and statistical analysis (devoted to [chaos machines](#chaos-machines-theorypdf)).*

---

Libchaos is a computing library written in the C++ language to help with the development of software for scientific research. The library tries to be as general as possible, modern and easy-to-use.

Project goal is to *implement & analyze* various algorithms for randomization and hashing, while maintaining simplicity and security, making them suitable for use in your own code. Popular tools like [TestU01](http://simul.iro.umontreal.ca/testu01/tu01.html), [Dieharder](https://www.phy.duke.edu/~rgb/General/dieharder.php) and [Hashdeep](https://github.com/jessek/hashdeep) are obsolete or their development has been stopped. Libchaos aims to replace them.

---

The library implements wide range of [chaos machines](#chaos-machines-theorypdf), which present the idea of creating a universal scheme with modular design and customizable parameters, which can be applied wherever *randomness* and *sensitiveness* is needed (pseudo-random oracle).

It turns out that a pseudo-random function can be easily enriched by a [chaotic system](https://en.wikipedia.org/wiki/Chaos_theory), creating something completely new. What's interesting is that this scheme allows specification by three parameters: *execution time*, *period/memory required*, *initial secret key*.

**Getting Help**

If you have questions about the library, please be sure to check out the [API documentation](#). If you still have questions, reach out to us on [IRC](https://webchat.freenode.net/?channels=#libchaos) or post a question on [stackoverflow](http://stackoverflow.com/questions/tagged/libchaos) (using the ``libchaos`` tag).

**Reporting Bugs**

Please open a [GitHub Issue](https://github.com/maciejczyzewski/libchaos/issues) and include as much information as you can. If possible, provide sample code that illustrates the problem you're reporting. If you're seeing a bug only on a specific repository, please provide a link to it if possible.

*Do not open a GitHub Issue for help, only for bug reports.*

Overview
========

<img src="http://maciejczyzewski.me/libchaos/_images/ncg-distribution-rankit.png" width="50%" /><img src="http://maciejczyzewski.me/libchaos/_images/ncg-speed-benchmark.png" width="50%" />

A lot of research has gone into chaos and randomness theory. Development in computer software and applications continues to be very dynamic. Each software problem requires different tools and algorithms to solve it effectively and achieve best results. As a consequence, we witness the announcement of new projects in quick succession with multiple updates. *Engineer's problem is how to decide which method will suit his needs best.*

<table width="100%">
<tr>
<th width="50%">Example</th>
<th width="50%">Instructions</th>
</tr>
<tr>
<td><pre>#include &lt;iostream&gt;
#include &lt;chaos.h&gt;  // library header

// initialize chaos machine (64-bit version)
CHAOS_MACHINE_XORRING64 machine;

int main(void) {
  machine.set_space(100000);  // 2^6400000 period length
  machine &lt;&lt; 0x8a5cd789635d2dff; // initialize with seed
  while (true) putc_unlocked(machine.pull(), stdout);
}</pre></td>
<td>
Just copy, paste it into new file and prompt:

<br><br><pre>$ g++ -std=c++11 -lchaos example.cc
$ ./a.out | xxd -l 1024</pre>

See it run! Have fun.
</td>
</tr>
</table>

This project offers very convenient interface for practical use, not just research. In the next section, there is a list of recommended algorithms and their strengths, period lengths, and speeds.

<table width="100%">
<tr>
<th width="50%">Example</th>
<th width="50%">Instructions</th>
</tr>
<tr>
<td><pre>#include &lt;iostream&gt;
#include &lt;chaos.h&gt;  // library header

// initialize PRNG (by Sebastiano Vigna)
CHAOS_PRNG_XOROSHIRO1024STAR prng;

int main(void) {
  prng &lt;&lt; 0x8a5cd789635d2dff; // initialize with seed
  while (true) putc_unlocked(prng.next(), stdout);
}</pre></td>
<td>
Notice that you can use <code>CHAOS_PRNG_*</code> and <code>CHAOS_MACHINE_*</code> algorithms. See the table below and read the library user guide to understand the difference.
</td>
</tr>
</table>

Pseudo-Random Number Generators
-------------------------------

PRNGs are algorithms for **generating a sequence of numbers** whose properties approximate the properties of sequences of random numbers. Generated sequence is **not truly random**, because it is completely determined by a relatively small set of initial values, called the seed.

<table width="100%">
<tr>
<th width="30%">Name</th>
<th>Seed</th>
<th>Output</th>
<th>Period</th>
<th>Quality</th>
<th>Speed</th>
</tr>
<tr>
<td><code>CHAOS_PRNG_KISS</code></td>
<td>uint32_t [2]</td>
<td>uint32_t</td>
<td>2<sup>119.5</sup></td>
<td>high</td>
<td>99.9175M items/s</td>
</tr>
<tr>
<td><code>CHAOS_PRNG_ABYSSINIAN</code></td>
<td>uint32_t [2]</td>
<td>uint32_t</td>
<td>2<sup>126</sup></td>
<td>high</td>
<td>165.754M items/s</td>
</tr>
<tr>
<td><code>CHAOS_PRNG_XOROSHIRO128PLUS</code></td>
<td>uint64_t</td>
<td>uint64_t</td>
<td>2<sup>128</sup></td>
<td>high</td>
<td>178.426M items/s</td>
</tr>
<tr>
<td><code>CHAOS_PRNG_XOROSHIRO1024STAR</code></td>
<td>uint64_t</td>
<td>uint64_t</td>
<td>2<sup>1024</sup></td>
<td>high</td>
<td>165.289M items/s</td>
</tr>
<tr>
<td><code>CHAOS_PRNG_XORSHF96</code></td>
<td>uint32_t [3]</td>
<td>uint32_t</td>
<td>2<sup>96</sup> - 1</td>
<td>high</td>
<td>185.148M items/s</td>
</tr>
</table>

**STL Comparison**

Library competes with PRNGs [from standard library](http://en.cppreference.com/w/cpp/numeric/random). Benchmarks [confirms](https://github.com/maciejczyzewski/libchaos/blob/master/tests/b_generators.cc#L68) that libchaos is faster and offers more...

```
Benchmark                                Time           CPU Iterations                   Speed (Travis)
-------------------------------------------------------------------------------------------------------
B__STL_MINSTD_RAND__Next               333 us        333 us       2108   1.46467GB/s   46.8694M items/s
B__STL_MT19937__Next                   229 us        229 us       3074   2.13247GB/s    68.239M items/s
B__STL_MT19937_64__Next                260 us        260 us       2683   1.87711GB/s   60.0674M items/s
B__STL_RANLUX24__Next                 2326 us       2327 us        295   214.828MB/s   6.71339M items/s
B__STL_RANLUX48__Next               118524 us     118598 us          6   67.4546MB/s   2.10796M items/s
B__STL_KNUTH_B__Next                  1083 us       1084 us        641   461.418MB/s   14.4193M items/s
```

Chaos Machines [(theory/pdf)](https://eprint.iacr.org/2016/468)
--------------

In mathematics, a chaos machine is a class of algorithms constructed on the base of chaos theory (mainly deterministic chaos) to produce **pseudo-random oracle**. It was designed specifically to combine the benefits of *hash function* and *pseudo-random function*.

<table width="100%">
<tr>
<th rowspan="2" width="100%">Name</th>
<th rowspan="2">Input</th>
<th rowspan="2">Output</th>
<th rowspan="2">Period</th>
<th rowspan="2">Quality</th>
<th colspan="2">Speed</th>
</tr>
<tr>
<th>Push</th><th>Pull</th>
</tr>
<tr>
<td><code>CHAOS_MACHINE_NCG</code> proof of concept</td>
<td>uint32_t</td>
<td>uint32_t</td>
<td>(2<sup>16n</sup>,2<sup>32n</sup>)</td>
<td>high</td>
<td>4.38007M<br>items/s</td>
<td>3.49098M<br>items/s</td>
</tr>
<tr>
<td><code>CHAOS_MACHINE_XORRING32</code></td>
<td>uint32_t</td>
<td>uint32_t</td>
<td>2<sup>32n</sup></td>
<td>high</td>
<td>33.0828M<br>items/s</td>
<td>48.938M<br>items/s</td>
</tr>
<tr>
<td><code>CHAOS_MACHINE_XORRING64</code></td>
<td>uint64_t</td>
<td>uint64_t</td>
<td>2<sup>64n</sup></td>
<td>high</td>
<td>31.6051M<br>items/s</td>
<td>41.6355M<br>items/s</td>
</tr>
</table>

Machines can be used to implement many cryptographic primitives, including cryptographic hashes, message authentication codes and randomness extractors.

Features
========

<table width="100%">
<tr>
<th width="30%">Ready</th>
<th width="30%">In Progress</th>
<th width="30%">TODO</th>
</tr>
<tr>
<td>Chaos machines</td>
<td>Statistical tests & functions</td>
<td>Entropy pools (different seeding scenarios)</td>
</tr>
<tr>
<td>Pseudo-random number generators</td>
<td>Adapter for truely random generators (dynamical reseeding)</td>
<td>Gnuplot utilities & analysis</td>
</tr>
<tr>
<td>Signatures for algorithms (portability on various architectures)</td>
<td>Ranking for algorithms (speed/quality)</td>
<td>Javascript API</td>
</tr>
<tr>
<td>STL compatibility</td>
<td>Hashdeep fork (named chaosdeep)</td>
<td>Seed recovery (attacks)</td>
</tr>
</table>

Installation
============

The library needs a C++ compiler that supports C++11. You will need *g++ 4.7* or newer to get started, so be sure to have an up-to-date compiler.

> **TODO.** Add this library to [Homebrew](http://brew.sh/). Then simply ``$ brew install libchaos`` to install this package on OS X. This is an idea for the future, to be realized when the library has a stable version.

Basic Installation
------------------

```
$ git clone git@github.com:maciejczyzewski/libchaos.git
$ cd libchaos && ./install.sh
```

Manual Installation
-------------------

We welcome patches. If you plan to contribute a patch, you need to build library and its own tests, which has further requirements:

- [Google Test](https://github.com/google/googletest) and [Google Benchmark](https://github.com/google/benchmark) (automatically downloaded as submodule)
- [CMake](https://cmake.org/) in version *2.8.7* (library uses [C++11](http://www.stroustrup.com/C++11FAQ.html))

```
$ git clone --recursive git@github.com:maciejczyzewski/libchaos.git
$ mkdir libchaos/build && cd "$_"
$ cmake -DLIBCHAOS_ENABLE_TESTING=ON ..
$ make && make install
```

Uninstalling Library
--------------------

CMake creates a file called ``install_manifest.txt`` when executing the install target. This contains a list of all installed files.

```
$ make uninstall # where is install_manifest.txt
```

Quick Start
===========

This page gives a good introduction to libchaos. It assumes you already have library installed. If you do not, head over to the [installation](#installation) section.

**A Minimal Program**

```C++
#include <iostream>
#include <chaos.h>  // library header

// initialize chaos machine (64-bit version)
CHAOS_MACHINE_XORRING64 machine;

int main(void) {
  machine.set_space(100000);  // 2^6400000 period length
  machine << 0x8a5cd789635d2dff; // initialize with seed
  while (true) putc_unlocked(machine.pull(), stdout);
}
```

**Interface: Chaos Machine**

```C++
#include <chaos.h>

CHAOS_MACHINE_XORRING64 machine;

int main(void) {
  uint64_t a = 0x84242f96eca9c41d, b = 0xa3c65b8776f96855, c;

  // PUSH
  machine.push(a);
  machine << b;

  // PULL
  machine.pull();
  c << machine;
  machine();
}
```

**Interface: Pseudo-Random Number Generator**

```C++
#include <chaos.h>

CHAOS_PRNG_XOROSHIRO128PLUS prng;

int main(void) {
  uint64_t a = 0x84242f96eca9c41d, b = 0xa3c65b8776f96855, c;

  // SEED
  prng.seed(a);
  prng << b;

  // NEXT
  prng.next();
  c << prng;
  prng();
}
```

**Normal Distribution**

```C++
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

  for (int n = 0; n < 5 * 10000; ++n) ++hist[std::round(d(gen))];

  for (auto p : hist)
    std::cout << std::fixed << std::setprecision(1) << std::setw(2) << p.first
              << ' ' << std::string(p.second / 200, '*') << '\n';
}
```

**Truely Random**

```C++
#include <chaos.h>

chaos::truely<CHAOS_MACHINE_XORRING64, std::random_device> gen;

int main(void) {
  for (size_t i = 0; i < 30; i += 3)
    printf("%p\t%p\t\%p\n", gen(), gen(), gen());
}
```

**File Checksum**

```C++
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <chaos.h>

CHAOS_MACHINE_XORRING64 gen;

void READ(const char* filename) {
  std::ifstream ifs(filename);
  std::vector<CHAOS_MACHINE_XORRING64::size_push> vec;

  vec.assign((std::istreambuf_iterator<char>(ifs)),
             (std::istreambuf_iterator<char>()));

  for (auto block : vec) gen.push(block);
}

int main(int argc, char* argv[]) {
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
```

**Libchaos: Battery**

```C++
#include <chaos.h>
#include <iostream>

// Chaos Machines

CHAOS_MACHINE_NCG x_0;
CHAOS_MACHINE_XORRING64 x_1;

// Pseudo-Random Number Generators

chaos::generators::xorshift<uint32_t, 1, 3, 10> x_2;
CHAOS_PRNG_ABYSSINIAN x_3;
chaos::generators::lcg<uint32_t, 48271, 0, 2147483647> x_4;
CHAOS_PRNG_KISS x_5;

// Adapters (double is universal in this case)

double UNIF01_NEXT_ADAPTER0() { return CHAOS_DOUBLE_U32(x_0.pull()); }
double UNIF01_NEXT_ADAPTER1() { return CHAOS_DOUBLE_U64(x_1.pull()); }
double UNIF01_NEXT_ADAPTER2() { return CHAOS_DOUBLE_U32(x_2.next()); }
double UNIF01_NEXT_ADAPTER3() { return CHAOS_DOUBLE_U32(x_3.next()); }
double UNIF01_NEXT_ADAPTER4() { return CHAOS_DOUBLE_U32(x_4.next()); }
double UNIF01_NEXT_ADAPTER5() { return CHAOS_DOUBLE_U32(x_5.next()); }

int main(void) {
  std::cout << "=== XORRING ==================================================="
            << std::endl;
  chaos::analysis gen1(UNIF01_NEXT_ADAPTER1);
  gen1.raport();
  std::cout << "=== XORSHIFT =================================================="
            << std::endl;
  chaos::analysis gen2(UNIF01_NEXT_ADAPTER2);
  gen2.raport();
  std::cout << "=== ABYSSINIAN ================================================"
            << std::endl;
  chaos::analysis gen3(UNIF01_NEXT_ADAPTER3);
  gen3.raport();
  std::cout << "=== KISS ======================================================"
            << std::endl;
  chaos::analysis gen5(UNIF01_NEXT_ADAPTER5);
  gen5.raport();
  std::cout << "=== LCG ======================================================="
            << std::endl;
  chaos::analysis gen4(UNIF01_NEXT_ADAPTER4);
  gen4.raport();
  std::cout << "=== NCG ======================================================="
            << std::endl;
  chaos::analysis gen0(UNIF01_NEXT_ADAPTER0);
  gen0.raport();
}
```

**TestU01: Battery**

```C++
#include <chaos.h>
#include <iostream>

extern "C" {
#include "bbattery.h"
#include "smarsa.h"
#include "unif01.h"
#include "util.h"
}

// Chaos Machines

CHAOS_MACHINE_NCG x_0;
CHAOS_MACHINE_XORRING64 x_1;

// Pseudo-Random Number Generators

chaos::generators::xorshift<uint32_t, 1, 3, 10> x_2;
CHAOS_PRNG_ABYSSINIAN x_3;
chaos::generators::lcg<uint32_t, 48271, 0, 2147483647> x_4;
CHAOS_PRNG_KISS x_5;

// Adapters (double is universal in this case)

double UNIF01_NEXT_ADAPTER0() { return CHAOS_DOUBLE_U32(x_0.pull()); }
double UNIF01_NEXT_ADAPTER1() { return CHAOS_DOUBLE_U64(x_1.pull()); }
double UNIF01_NEXT_ADAPTER2() { return CHAOS_DOUBLE_U32(x_2.next()); }
double UNIF01_NEXT_ADAPTER3() { return CHAOS_DOUBLE_U32(x_3.next()); }
double UNIF01_NEXT_ADAPTER4() { return CHAOS_DOUBLE_U32(x_4.next()); }
double UNIF01_NEXT_ADAPTER5() { return CHAOS_DOUBLE_U32(x_5.next()); }

int main(void) {
	unif01_Gen *gen;
	gen = unif01_CreateExternGen01(CHAOS_META_NAME(CHAOS_MACHINE_XORRING64),
	                               UNIF01_NEXT_ADAPTER1);

	// smarsa_BirthdaySpacings(gen, NULL, 1, 1000, 0, 10000, 2, 1);
	// smarsa_BirthdaySpacings(gen, NULL, 1, 10000, 0, 1000000, 2, 1);

	// smarsa_BirthdaySpacings(gen, NULL, 3, 200000, 14, 256, 8, 1);
	// smarsa_BirthdaySpacings(gen, NULL, 3, 20000000, 14, 256, 8, 1);

	// bbattery_BlockAlphabit(gen, 1024 * 1024, 0, 8);
	// bbattery_Alphabit(gen, 1024 * 1024, 0, 8);

	// bbattery_FIPS_140_2(gen);
	// bbattery_Rabbit(gen, 10000000);
	bbattery_pseudoDIEHARD(gen);

	// bbattery_SmallCrush(gen);
	// bbattery_Crush(gen);
	// bbattery_BigCrush(gen);

	unif01_DeleteExternGenBits(gen);
}
```

User Guide
==========

> **Warning.** This project is at an early stage of development, every piece of hardware and software is in alpha version, if you are an adventurer this is a place for you! The list of core features is not ready. You need to dive into source code to find out what is working properly. If you have some ideas, feel free to contribute.

**Structure**

Directory structure and contents:

| Directory                   | Purpose                                                                       |
|-----------------------------|-------------------------------------------------------------------------------|
| `deps/`                     | Contains non-installed 3rd-party code that library depends on.                |
| `cmake/`                    | Miscellaneous helper code.                                                    |
| `include/chaos/engines/`    | Implementations of chaos machines.                                            |
| `include/chaos/generators/` | Implementations of pseudo-random number generators.                           |
| `include/chaos/`            | Core library abstractions.                                                    |
| `tests`                     | Benchmarks and tests (using [Google Test](https://github.com/google/googletest) and [Google Benchmark](https://github.com/google/benchmark)). |

Contributing
============

> Have you ever been fascinated by chaotic maps, fractals or something connected with disorder or randomness? Join us or support by sharing!

Our work flow is a [typical GitHub flow](https://guides.github.com/introduction/flow/index.html), where contributors fork the [libchaos repository](https://github.com/maciejczyzewski/libchaos), make their changes on branch, and submit a [Pull Request](https://help.github.com/articles/using-pull-requests) (a.k.a. "PR"). Pull requests should usually be targeted at the `master` branch.

Please include a nice description of your changes when you submit your PR; if we have to read the whole diff to figure out why you're contributing, you're less likely to get feedback and have your change merged in. Life will be a lot easier for us if you follow this pattern (i.e. fork, named branch, submit PR). If you use your fork's `master` branch directly, things can get messy.

Before wrapping up a PR, you should be sure to:

* Write tests to cover any functional changes.
* Update documentation for any changed public APIs.
* Add to the [CHANGELOG.md](https://raw.githubusercontent.com/maciejczyzewski/libchaos/master/CHANGELOG.md) file information describing any major changes.

Testing & Benchmarking
----------------------

To execute tests you need to build library manualy with option ``-DLIBCHAOS_ENABLE_TESTING=ON``.

```bash
tests/
    - b_<filename>.cc               # for benchmarking      |       BENCHMARK(TEST_NAME)
    - t_<filename>.cc               # for testing           |       TEST(TEST_NAME, CASE_NAME)
```

After ``make build``, repository should contains programs named as: ``bchaos`` and ``tchaos`` (separately for benchmark and test), because tests are *required* while benchmarks are *optional*.

Future Ideas
------------

If you are starting to work on a particular area, feel free to submit a PR that highlights your work in progress (and note in the PR title that it's not ready to merge). These early PRs are welcome and will help in getting visibility for your fix, allow others to comment early on the changes and also let others know that you are currently working on something.

License
=======

See LICENSE file in this repository.
