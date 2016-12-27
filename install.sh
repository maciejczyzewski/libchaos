#!/bin/bash
rm -rf CMakeCache.txt CMakeFiles
mkdir -p build && cd build && cmake -DLIBCHAOS_ENABLE_TESTING=OFF .. \
	&& make && make install
