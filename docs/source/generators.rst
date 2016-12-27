.. role:: okay
.. role:: poor
.. role:: vbad

.. _prngs:

Pseudo-Random Number Generator
==============================

PRNG is an algorithm for **generating a sequence of numbers** whose properties approximate the properties of sequences of random numbers. Generated sequence is **not truly random**, because it is completely determined by a relatively small set of initial values, called the seed.

List of Generators
------------------

Abyssinian
^^^^^^^^^^

.. class:: chaos::generators::abyssinian

+--------------------+----------+-----------------+--------------+--------------+
| Seed               | Output   | Period          | Quality      | Speed        |
+====================+==========+=================+==============+==============+
| uint32_t, uint32_t | uint32_t | :math:`2^{126}` | :okay:`high` | :okay:`high` |
+--------------------+----------+-----------------+--------------+--------------+

Xoroshiro128+
^^^^^^^^^^^^^

.. class:: chaos::generators::xoroshiro128plus

+----------+----------+-----------------+--------------+--------------+
| Seed     | Output   | Period          | Quality      | Speed        |
+==========+==========+=================+==============+==============+
| uint64_t | uint64_t | :math:`2^{128}` | :okay:`high` | :okay:`high` |
+----------+----------+-----------------+--------------+--------------+

Xoroshiro1024*
^^^^^^^^^^^^^^

.. class:: chaos::generators::xoroshiro1024star

+----------+----------+------------------+--------------+--------------+
| Seed     | Output   | Period           | Quality      | Speed        |
+==========+==========+==================+==============+==============+
| uint64_t | uint64_t | :math:`2^{1024}` | :okay:`high` | :okay:`high` |
+----------+----------+------------------+--------------+--------------+

KISS
^^^^^^^^

.. class:: chaos::generators::kiss

https://eprint.iacr.org/2011/007.pdf

+--------------------+----------+-------------------+--------------+--------------+
| Seed               | Output   | Period            | Quality      | Speed        |
+====================+==========+===================+==============+==============+
| uint32_t, uint32_t | uint32_t | :math:`2^{119.5}` | :okay:`high` | :okay:`high` |
+--------------------+----------+-------------------+--------------+--------------+

.. Linear Congruential Generator
.. ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. .. class:: chaos::generators::lcg<utype, a, c, m>

.. https://en.wikipedia.org/wiki/Linear_congruential_generator

.. +------+--------+---------------+-------------+--------------+
.. | Seed | Output | Period        | Quality     | Speed        |
.. +======+========+===============+=============+==============+
.. | ???  | ???    | :math:`2^{n}` | :vbad:`low` | :okay:`high` |
.. +------+--------+---------------+-------------+--------------+

.. Xorshift
.. ^^^^^^^^

.. .. class:: chaos::generators::xorshift<utype, a, b, c>

.. http://www.jstatsoft.org/v08/i14/paper

.. +-------+--------+---------------+----------------+--------------+
.. | Seed  | Output | Period        | Quality        | Speed        |
.. +=======+========+===============+================+==============+
.. | ???   | ???    | :math:`2^{n}` | :poor:`medium` | :okay:`high` |
.. +-------+--------+---------------+----------------+--------------+

Adapter’s Interface
-------------------

Typical Case
^^^^^^^^^^^^

.. code-block:: c++

	CHAOS_PRNG_XOROSHIRO128PLUS gen; // shortcut (default seed)
	chaos::prng<chaos::generators::xoroshiro128plus> gen_class;
	// above: gen == gen_class

	// seeding is optional
	gen.seed(0x8a5cd789635d2dff);

	gen.next(); // return number
	gen.next(); // and another...

Advanced Example
^^^^^^^^^^^^^^^^

.. code-block:: c++

	CHAOS_PRNG_KISS gen;

	CHAOS_PRNG_KISS::size_next out; // for @3
	// uint32_t == CHAOS_PRNG_KISS::size_next

	// seed
	gen.seed(0x11223344); // @1
	gen << 0x11223344;    // @2

	// next
	gen.next();           // @1
	gen();                // @2
	gen >> out;           // @3

.. Customizable Template
.. ^^^^^^^^^^^^^^^^^^^^^

.. .. code-block:: c++

.. 	chaos::generators::xorshift<uint32_t, 1, 3, 10> gen;
.. 	gen.__seed(0x8a5cd789); // like normal generator
.. 	gen.__next(); // returns uint32_t

