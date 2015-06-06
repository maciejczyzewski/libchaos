.PHONY: build analyze

SEED = 1013904223

build:
	mkdir -p bin
	gcc tests/hash.c -o bin/hash
	gcc tests/random.c -o bin/random

random: bin/random
	./$< $(SEED) | head | xxd

analyze: bin/random
	./$< $(SEED) | pv | dieharder -a -g 200