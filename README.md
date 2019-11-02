<a href="https://github.com/maciejczyzewski/libchaos">
    <img src="http://maciejczyzewski.me/libchaos/_static/libchaos.svg" alt="libchaos"
         title="libchaos" align="right" width="100" height="100" />
</a>
[![Current Release](https://img.shields.io/github/release/maciejczyzewski/libchaos.svg)](https://github.com/maciejczyzewski/libchaos/releases)
[![Build Status](https://travis-ci.org/maciejczyzewski/libchaos.svg?branch=master)](https://travis-ci.org/maciejczyzewski/libchaos)

[Features](#features) |
[Overview](#overview) |
[Installation](#installation) |
[User Guide](#user-guide) |
[Contributing](#contributing) |
[License](#license)

**Got a question?** Join us on [stackoverflow](http://stackoverflow.com/questions/tagged/libchaos) or chat on [IRC](https://webchat.freenode.net/?channels=#libchaos).

*Advanced library for randomization, hashing and statistical analysis (devoted to [chaos machines](https://en.wikipedia.org/wiki/Chaos_machine)).*

---

Libchaos is a computing library written in the C++ language to help with the development of software for scientific research. The library tries to be as general as possible, modern and easy-to-use.

Project goal is to *implement & analyze* various algorithms for randomization and hashing, while maintaining simplicity and security, making them suitable for use in your own code. Popular tools like [TestU01](http://simul.iro.umontreal.ca/testu01/tu01.html), [Dieharder](https://www.phy.duke.edu/~rgb/General/dieharder.php) and [Hashdeep](https://github.com/jessek/hashdeep) are obsolete or their development has been stopped. Libchaos aims to replace them.

---

The library implements wide range of [chaos machines](#chaos-machines-theorypdf), which present the idea of creating a universal scheme with modular design and customizable parameters, which can be applied wherever *randomness* and *sensitiveness* is needed (pseudo-random oracle).

```C++
std::string hash = chaos::password<CHAOS_MACHINE_XORRING64, 500, 50, 95>("my password", "private salt");
// hash == "EA79560A7E0937EC66BDBE22EAFDC96AA5D7AFE4E970C3C856B7D92632EAA8EEC828E61E59E024922F58D4045BC053E"
```

It turns out that a pseudo-random function can be easily enriched by a [chaotic system](https://en.wikipedia.org/wiki/Chaos_theory), creating something completely new. What's interesting is that this scheme allows specification by three parameters: *execution time*, *period/memory required*, *initial secret key*.

---

**Getting Help**

If you have questions about the library, please be sure to check out the [API documentation](#). If you still have questions, reach out to us on [IRC](https://webchat.freenode.net/?channels=#libchaos) or post a question on [stackoverflow](http://stackoverflow.com/questions/tagged/libchaos) (using the ``libchaos`` tag).

**Reporting Bugs**

Please open a [GitHub Issue](https://github.com/maciejczyzewski/libchaos/issues) and include as much information as you can. If possible, provide sample code that illustrates the problem you're reporting. If you're seeing a bug only on a specific repository, please provide a link to it if possible.

*Do not open a GitHub Issue for help, only for bug reports.*

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
<td>Adapter for truly random generators (dynamical reseeding)</td>
<td>Gnuplot/R utilities & analysis</td>
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

Overview
========

![benchmark](http://maciejczyzewski.me/libchaos/_images/benchmark.svg)

Each software problem requires different tools and algorithms to solve it effectively and achieve best results. *Engineer's problem is how to decide which method will suit his needs best.*

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
  machine.set_space(100000); // 2^6400000 period length
  machine.push(0x8a5cd789635d2dff);    // add some data
  machine.push(0x284600e3f30e38c3);     // and other...
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
  prng.seed(0x8a5cd789635d2dff); // initialize with seed
  while (true) putc_unlocked(prng.next(), stdout);
}</pre></td>
<td>
Notice that you can use <code>CHAOS_PRNG_*</code> and <code>CHAOS_MACHINE_*</code> algorithms. See the table below and read the library user guide to understand the difference.
</td>
</tr>
</table>

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

User Guide
==========

This page gives a good introduction to libchaos. It assumes you already have library installed. If you do not, head over to the [installation](#installation) section. More complicated examples are in [`examples/` directory](https://github.com/maciejczyzewski/libchaos/tree/master/examples).

> **Warning.** This project is at an early stage of development. The list of core features is not ready. Only main modules are documented. If you have some ideas, feel free to contribute.

Structure
---------

Directory structure and contents:

| Directory                   | Purpose                                                                       |
|-----------------------------|-------------------------------------------------------------------------------|
| `deps/`                     | Contains non-installed 3rd-party code that library depends on.                |
| `cmake/`                    | Miscellaneous helper code.                                                    |
| `include/chaos/engines/`    | Implementations of chaos machines.                                            |
| `include/chaos/generators/` | Implementations of pseudo-random number generators.                           |
| `include/chaos/`            | Core library abstractions.                                                    |
| `tests`                     | Benchmarks and tests (using [Google Test](https://github.com/google/googletest) and [Google Benchmark](https://github.com/google/benchmark)). |

Algorithms
----------

List of implemented algorithms is in the [`ALGORITHMS`](https://github.com/maciejczyzewski/libchaos/blob/master/ALGORITHMS) file.

```C++
CHAOS_MACHINE_* gen; // @1: for CMs
CHAOS_PRNG_* gen;    // @2: for PRNGS
```

**Chaos Machines**

Settings:

```C++
gen.set_time(4);      // time parameter
gen.set_space(1000);  // memory parameter (period length)
                      // initial secret key
gen.set_key({0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
             0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
             0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
             0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
             0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca});
```

For pushing a blob:

```C++
gen.push(a); // @1: using function (fastest)
gen << a;    // @2: using operator
```

For getting a block:

```C++
gen.pull();  // @1: using function (fastest)
gen >> a;    // @2: using operator
gen();       // @3: STL style (for API)
```

For clearing to initial state:

```C++
gen.reset();
```

**Pseudo-Random Number Generators**

For seeding:

```C++
gen.seed(a); // @1: using function (fastest)
gen << a;    // @2: using operator
```

For getting a number:

```C++
gen.next();  // @1: using function (fastest)
gen >> a;    // @2: using operator
gen();       // @3: STL style (for API)
```

**Password Salting**

For salting passwords:

```C++
std::string hash =
    chaos::password<
        CHAOS_MACHINE_XORRING64,   // machine algorithm
        time_parameter,            // computational consumption
        space_parameter,           // memory required/space-hardness
        output_size                // length of hashed password
    >(password, salt);
```

Example usage:

```C++
std::string hash = chaos::password<CHAOS_MACHINE_XORRING64, 200, 50, 40>(
    "some secret password", "my private salt");
// hash == "C6EB731977A6CB785196BAF11CA8BEC02CDEAFE8"
```

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

See [`LICENSE`](https://github.com/maciejczyzewski/libchaos/blob/master/LICENSE) file in this repository.
