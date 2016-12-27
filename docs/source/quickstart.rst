Quickstart
==========

This page gives a good introduction to libchaos. It assumes you already have library installed. If you do not, head over to the :ref:`installation` section.

A Minimal Program
-----------------

The library is very simple to use. As a short example, this is how it could be used to initialize chaos machine, sent and receive some bytes, and finally analyze it:

.. code-block:: c++

	#include <iostream>
	#include <string>
	#include <vector>

	#include <chaos.h> // library header
										 // namespace chaos::*

	// allocate std::vector (our starting variable)
	std::vector<uint8_t> y_secret = {0x14, 0x15, 0x92, 0x65,
																	 0x35, 0x89, 0x79, 0x32};

	// initialize chaos machine using NCG algorithm/engine
	chaos::machine<chaos::engines::ncg> x_machine(y_secret);

	int main(void) {
		// allocate std::vector (our message/bitstrings)
		std::string y_string = "Lorem ipsum dolor sit...";
		std::vector<uint8_t> y_vector(y_string.begin(), y_string.end());

		// make use of chaos machine (push/pull interface)
		x_machine.message(y_vector);                           // push
		std::vector<uint8_t> y_result = x_machine.digest(256); // pull
		std::sort(y_result.begin(), y_result.end());           // sort

		// print sorted for rank-it plot (analysis)
		for (size_t i = 0; i < y_result.size(); i++)
			printf("%zu, %d\n", i, y_result[i]);
	}

That code can be compiled simply by linking our library:

.. code-block:: console

	$ g++ example.cpp -std=c++11 -lchaos -o example

Libchaos only provides data and results. So we need data visualation tool to see it readable. We can use `gnuplot <http://www.gnuplot.info/>`_ (portable command-line driven graphing utility). Simple code to generate `rank-it plot <https://en.wikipedia.org/wiki/Rankit>`_:

.. code-block:: gnuplot

	# filename: rankit.plt

	set terminal svg
	set output 'result.svg'

	set xrange [0:256]
	set yrange [0:256]

	plot x linecolor rgb "red" notitle
	plot 'points_x_y.csv' using 1:2 with points notitle

Then we use command-line utilities:

.. code-block:: console

	$ ./example > points_x_y.csv
	$ gnuplot rankit.plt

Final result (data visualization):

.. image:: _static/ncg_basic_rankit.svg
	 :width: 100%

Testing & Benchmarking
----------------------

Project uses `Google Test <https://github.com/google/googletest>`_ and `Google Benchmark <https://github.com/google/benchmark>`_. To execute tests you need to build library manualy with option ``-DLIBCHAOS_ENABLE_TESTING=ON``.

.. code-block:: console

	tests/
		- b_<filename>.cc  # for benchmarking    |  BENCHMARK(TEST_NAME)
		- t_<filename>.cc  # for testing         |  TEST(TEST_NAME, CASE_NAME)

After ``make build``, repository should contains programs named as: ``bchaos`` and ``tchaos`` (separately for benchmark and test), because tests are *required* while benchmarks are *optional*.

Contributing To Project
-----------------------

Our work flow is a `typical GitHub flow <https://guides.github.com/introduction/flow/index.html>`_, where contributors fork the `libchaos repository <https://github.com/maciejczyzewski/libchaos>`_, make their changes on branch, and submit a `Pull Request <https://help.github.com/articles/using-pull-requests>`_ (a.k.a. "PR"). Pull requests should usually be targeted at the `master` branch.

Please include a nice description of your changes when you submit your PR; if we have to read the whole diff to figure out why you're contributing in the first place, you're less likely to get feedback and have your change merged in.

Life will be a lot easier for you if you follow this pattern (i.e. fork, named branch, submit PR). If you use your fork's `master` branch directly, things can get messy.

Before wrapping up a PR, you should be sure to:

* Write tests to cover any functional changes.
* Update documentation for any changed public APIs.
* Add to the `CHANGELOG.md <https://raw.githubusercontent.com/maciejczyzewski/libchaos/master/CHANGELOG.md>`_ file describing any major changes.

Future Ideas
^^^^^^^^^^^^

If you are starting to work on a particular area, feel free to submit a PR that highlights your work in progress (and note in the PR title that it's not ready to merge). These early PRs are welcome and will help in getting visibility for your fix, allow others to comment early on the changes and also let others know that you are currently working on something.
