.. role:: okay
.. role:: poor
.. role:: vbad

.. _chaos-machines:

Chaos Machines
==============

In mathematics, a chaos machine is a class of algorithms constructed on the base of chaos theory (mainly deterministic chaos) to produce **pseudo-random oracle**. It presents the idea to create a universal scheme with modular design and customizable parameters, that can be applied where **randomness** and **sensitiveness** is needed.

It was designed specifically to combine the benefits of hash function and pseudo-random function. However, it can be used to implement many cryptographic primitives, including cryptographic hashes, message authentication codes and randomness extractors.

List of Engines
---------------

There is an empty template for chaos machine, read `source code <https://github.com/maciejczyzewski/libchaos/blob/master/src/engines/empty.cc>`_ from repository.

Naive Czyzewski Generator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. class:: chaos::engines::ncg

https://eprint.iacr.org/2016/468 `(latest version) <https://github.com/maciejczyzewski/libchaos/blob/master/docs/paper/document.pdf>`_

**Purpose:** randomness extractor, cryptographic primitives

+-------------------------------------------------+---------------------------------------------+
| .. image:: _static/ncg-distribution-rankit.png  | .. image:: _static/ncg-speed-benchmark.png  |
|   :alt: rank-it plot of dist.                   |   :alt: benchmark of push function          |
+-------------------------------------------------+---------------------------------------------+
| Compare with these ones discussed on Reddit_.   | Push is very slow on big space parameter.   |
+-------------------------------------------------+---------------------------------------------+

.. _Reddit: https://www.reddit.com/r/crypto/comments/2d4m1v/how_random_is_your_data_on_interpreting_diehard/

Emphasis has been placed on period that is calculable, but also on high sensitivity to initial conditions
and quality of output. Algorithm passes all the Dieharder, NIST and TestU01 test sets. In addition, it shows resistance to common cryptographic attacks.

+----------+----------+-----+----------------------------+----------------+----------------+---------------------------------+
| Input    | Output   | BPS | Period                     | Security       | Quality        | Speed                           |
+==========+==========+=====+============================+================+================+=================================+
| uint32_t | uint32_t | 4   | :math:`[2^{16m}, 2^{32m}]` | :okay:`high`   | :okay:`high`   | :poor:`medium` [#ncg-drawback]_ |
+----------+----------+-----+----------------------------+----------------+----------------+---------------------------------+

.. [#ncg-drawback] The drawback is the limited quantity of the machine parameters. On each push action, it engages all possible states from buffer space. Therefore, hashing for huge buffers does not make sense (computation complexity increase).

Xorspace
^^^^^^^^

.. class:: chaos::engines::xorspace

+----------+----------+-----+----------------------------+----------------+----------------+----------------+
| Input    | Output   | BPS | Period                     | Security       | Quality        | Speed          |
+==========+==========+=====+============================+================+================+================+
| uint8_t  | uint8_t  | 1   | ???                        | :vbad:`low`    | :poor:`medium` | :okay:`high`   |
+----------+----------+-----+----------------------------+----------------+----------------+----------------+

Adapter's Interface
-------------------

.. class:: chaos::machine

This module is a universal adapter for different types of chaos machine algorithms. It's compatible with
`STL pseudo-random number engines <http://en.cppreference.com/w/cpp/numeric/random>`_.

.. code-block:: c++

	#include <chaos.h>

	// case #1: with default starting variable
	chaos::machine<chaos::engines::ncg> x; // ncg.__default_key

	// case #2: using shortcut (same way as case #1)
	CHAOS_MACHINE_NCG y; // ncg.__default_key

	// case #3: with initial secret key (vector of machine::size_cell)
	std::vector<CHAOS_MACHINE_NCG::size_cell> secret_key = {0x11, 0x22};
	CHAOS_MACHINE_NCG z(secret_key);

Parameters Settings
^^^^^^^^^^^^^^^^^^^

Selection of parameters provides preferred properties and security level. Therefore, machine contains three external variables: *initial secret key*, *time parameter*, and *space parameter*.

They can be controlled using functions listed below. Changing parameters, change properties for `specific engine <#list-of-engines>`_ (e.g. provide extra period or space-hardness).

.. function:: machine.set_key(std::vector<uint8_t>)

**Initial Secret Key** (Starting Variable). Is a fixed-size input to a chaos machine that is typically required to be random or pseudo-random. Setting the initial secret key is an example of using machine as the `MAC algorithm <https://en.wikipedia.org/wiki/Message_authentication_code>`_.

.. This function sets initial secret key to current instance of the chaos machine.

.. code-block:: c++

	x.set_key({0x11, 0x22, 0x33, 0x44});

.. function:: machine.set_space(size_t)

**Space Parameter** (Memory Cost). Defines the number of dynamical systems to be used in the machine. Concomitantly indicates how many bytes of working space the buffer will require during its computation, because each system needs his own space.

.. code-block:: c++

	x.set_space(8);

.. function:: machine.set_time(size_t)

**Time Parameter** (Time Cost). That determines the number of rounds of computation that machine performs. The larger the time parameter, the longer the output computation will take. As computational power increases, users can increase this time parameter to keep the number of wall-clock seconds required to compute each sequence near-constant.

.. code-block:: c++

	x.set_time(2);

Input Interface
^^^^^^^^^^^^^^^

Push function is primarily the input function of the machine, it absorbs bit string and uses in system changes. The results are used later by the pull function. The push procedure is a collection of operations which aims to control evolution functions. It modifies inflicted systems, based on initial secret key and input.

.. function:: machine.push(machine::size_push)

.. code-block:: c++

	x.push(0x11);

Output Interface
^^^^^^^^^^^^^^^^

The output of pull function is a bit string of fixed length, sequences of pseudo-random numbers that are unique and sensitive to the initial conditions.

.. function:: machine.pull(void) -> machine::size_pull

.. code-block:: c++

	uint8_t y_result = x.pull();

Resetting to Initial State
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. function:: machine.reset(void)

This function clears the memory, but holds last used parameters. After this, operation machine is in the initial state.

.. code-block:: c++

	x.reset();

Sample Applications
-------------------

Pseudo-random Function Family
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c++

	/*
	#include <ctime>
	#include <iostream>

	#include <chaos.h> // library header

	// create seed on the base of time
	uint8_t seed = static_cast<uint8_t>(time(NULL));

	// initialize chaos machine using NCG algorithm/engine
	chaos::machine<chaos::engines::ncg> x_machine;

	int main(void) {
		// configure machine with parameters (t=2, m=256)
		x_machine.set_time(2); x_machine.set_space(256);

		// initialize with seed (still pseudo-randomness)
		x_machine.push(seed); // can be anything

		// serve like /dev/random
		while (true)
			putc_unlocked(x_machine.pull(), stdout);
	}
	*/

	#include <ctime>
	#include <iostream>

	#include <chaos.h>  // library header

	// create seed on the base of time
	CHAOS_MACHINE_NCG::size_push seed =
			static_cast<CHAOS_MACHINE_NCG::size_push>(time(NULL));

	// initialize chaos machine using NCG algorithm/engine
	CHAOS_MACHINE_NCG x_machine;

	int main(void) {
		// configure machine with parameters (t=2, m=256)
		x_machine.set_time(2); x_machine.set_space(256);

		// initialize with seed (still pseudo-randomness)
		x_machine.push(seed);  // can be anything

		// serve like /dev/random
		while (true) putc_unlocked(x_machine.pull(), stdout);
	}

Execute and stop after 1024 octets:

.. code-block:: console

	$ ./fake_dev_random | xxd -l 1024

Cryptographic Hash Function
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c++

	/*
	#include <iostream>
	#include <string>
	#include <vector>

	#include <chaos.h> // library header

	// allocate std::vector (our starting variable)
	std::vector<uint8_t> y_secret = {0x14, 0x15, 0x92, 0x65,
																	 0x35, 0x89, 0x79, 0x32};

	// initialize chaos machine using NCG algorithm/engine
	chaos::machine<chaos::engines::ncg> x_machine(y_secret);

	int main(int argc, char** argv) {
		// allocate std::vector (our message/bitstrings)
		std::string y_string = argv[1]; // "Lorem ipsum dolor sit..."
		std::vector<uint8_t> y_vector(y_string.begin(), y_string.end());

		// make use of chaos machine (push/pull interface)
		x_machine.message(y_vector);                           // push
		std::vector<uint8_t> y_result = x_machine.digest(256); // pull

		// print values in hexadecimal format
		for (size_t i = 0; i < y_result.size(); i++)
			printf("%02x", y_result[i]);
	}
	*/

	#include <algorithm>
	#include <iostream>
	#include <string>
	#include <vector>

	#include <chaos.h>  // library header

	// allocate std::vector (our starting variable)
	std::vector<CHAOS_MACHINE_NCG::size_cell> y_secret = {0x14, 0x15, 0x92, 0x65,
																												0x35, 0x89, 0x79, 0x32};

	// initialize chaos machine using NCG algorithm/engine
	CHAOS_MACHINE_NCG x_machine(y_secret);

	int main(int argc, char** argv) {
		// allocate std::vector (our message/bitstrings)
		std::string y_string = argv[1];  // "Lorem ipsum dolor sit..."
		std::vector<CHAOS_MACHINE_NCG::size_push> y_vector(y_string.begin(),
																											 y_string.end());

		// [@1] push
		for (size_t i = 0; i < y_vector.size(); i++) x_machine.push(y_vector[i]);

		// [@2] pull
		std::vector<CHAOS_MACHINE_NCG::size_pull> y_result(128, 0);
		std::generate(y_result.begin(), y_result.end(),
									[&x_machine] { return x_machine.pull(); });

		// print values in hexadecimal format
		for (size_t i = 0; i < y_result.size(); i++) printf("%08x", y_result[i]);
	}

Simple comparison to check `avalanche effect <https://en.wikipedia.org/wiki/Avalanche_effect>`_:

.. code-block:: console

	$ cmp -bl <(./fake_hash_function "Lorem ipsum dolor sit...") \
						<(./fake_hash_function "Lorem ipsum bolor sit...")
