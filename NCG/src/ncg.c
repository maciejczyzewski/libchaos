/*  NCG written in 2015 by Maciej A. Czyzewski

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>.  */

#include <stdint.h>
#include <string.h>

// S - seed, I - increment, t - mask, i - temporary
uint32_t S, I, t, i;

// The length of the initial states
#define SIZE 16

// Abbreviation for getting values from the matrix
#define M(i) ((i) & (SIZE - 1))

// Bit rotation
#define R(x, y) (((x) << (y)) | ((x) >> (16 - (y))))

// XOR gate, relationships
#define L(x, y) {                                    \
  (y) ^= ((x) << 5) ^ ((y) >> 3) ^ ((x) >> 1);       \
  (x) ^= ((y) << 8) ^ ((x) << 3) ^ ((y) << 9);       \
}

// Variebles in the algorithm
uint16_t a, b, c, d;

// Initial state matrix (pi digits)
uint16_t G[SIZE], Q[SIZE] = { 1, 4, 1, 5, 9, 2, 6, 5,
                              3, 5, 8, 9, 7, 9, 3, 2 };

void push(uint32_t seed) {
  // Preparation
  I = seed * 0x3C6EF35F;

  for (S = seed, i = 0; i < SIZE; i++) {
    // Reinforcement
    G[M(i)] ^= (S * I - I ^ S) >> 16;
    G[M(i)] ^= (S * I + I ^ S) >> 00;

    // Finalization
    I ^= ((G[M(I - 1)] + G[M(i)]) << 16)
      ^  ((G[M(I + 1)] - G[M(i)]) << 00);
  }
}

uint32_t pull(void) {
  // Variebles
  a = G[M(I + 0)]; b = G[M(I + 1)];
  c = G[M(I - 2)]; d = G[M(I + 2)];

  // Initialization
  t = (a + I) * (b - S);

  // Allowance
  t ^= a ^ (b << 8) ^ (c << 16) ^ (d & 0xff) ^ ((d >> 8) << 24);

  // Mixing
  L(G[M(I + 0)], G[M(I - 2)]);
  L(G[M(I + 0)], G[M(I + 2)]);

  // Transformation
  G[M(I + 0)] = G[M(t - 1)] ^ R(c, M(t)) ^ R(d, M(t)) ^ a;
  G[M(I + 1)] = (b >> 1) ^ (-(b & 1u) & 0xB400u); // LFSR

  // Increment
  I += 2;

  return t;
}

void reset(void) {
  // Copying defaults
  memcpy(G, Q, 2 * SIZE);
}