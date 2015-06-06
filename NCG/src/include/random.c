/*  NCG written in 2015 by Maciej A. Czyzewski

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>.  */

void ncg(const uint32_t seed) {
  // Cleaning state matrix
  reset();

  // Push to NCG structure
  push(seed);
}