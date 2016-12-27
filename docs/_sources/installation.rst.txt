.. _installation:

Installation
============

Library needs C++ compiler that supports C++11. You will need *g++ 4.7* or newer to get started, so be sure to have an up-to-date compiler.

.. todo::
	Add this library to `Homebrew <http://brew.sh/>`_. Then simply ``$ brew install libchaos`` to install this package on OS X. This is future idea, when library will have stable version.

Basic Installation
------------------

.. code-block:: console

	$ git clone git@github.com:maciejczyzewski/libchaos.git
	$ cd libchaos && ./install.sh

Manual Installation
-------------------

We welcome patches. If you plan to contribute a patch, you need to build libchaos and its own tests, which has further requirements:

- `Google Test <https://github.com/google/googletest>`_ and `Google Benchmark <https://github.com/google/benchmark>`_ (automatically downloaded as submodule)
- `CMake <https://cmake.org/>`_ in version *2.8.7* (library uses `C++11 <http://www.stroustrup.com/C++11FAQ.html>`_)

.. code-block:: console

	$ git clone --recursive git@github.com:maciejczyzewski/libchaos.git
	$ mkdir libchaos/build && cd "$_"
	$ cmake -DLIBCHAOS_ENABLE_TESTING=ON ..
	$ make && make install

Uninstalling Library
--------------------

CMake creates a file called ``install_manifest.txt`` when executing the install target. This contains a list of all installed files.

.. code-block:: console

	$ make uninstall # where is install_manifest.txt
