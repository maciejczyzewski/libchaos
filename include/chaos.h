#ifndef CHAOS_H
#define CHAOS_H

// modules
#include <chaos/types.hh>
#include <chaos/macros.hh>
#include <chaos/analysis.hh>
#include <chaos/seed.hh>

// adapters
#include <chaos/machine.hh>
#include <chaos/truely.hh>
#include <chaos/prng.hh>

// engines
#include <chaos/engines/empty.hh>
#include <chaos/engines/ncg.hh>
#include <chaos/engines/xorring.hh>

// generators
#include <chaos/generators/abyssinian.hh>
#include <chaos/generators/kiss.hh>
#include <chaos/generators/linear.hh> // FIXME: move to mixins
#include <chaos/generators/xoroshiro.hh>
#include <chaos/generators/xorshift.hh> // FIXME: move to mixins
#include <chaos/generators/xorshf96.hh>

#endif // CHAOS_H
