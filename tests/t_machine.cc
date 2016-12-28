#include "gtest/gtest.h"

#include <iostream>
#include <vector>

#include <chaos.h>

/// machine ////////////////////////////////////////////////////////////////////

TEST(MACHINE, T__CHAOS_MACHINE__AdapterInitialization) {
	CHAOS_MACHINE_XORRING64 gen_1;

	gen_1.set_time(4);
	gen_1.set_space(1000);
	gen_1.set_key({0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
	               0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
	               0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
	               0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
	               0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca});
	gen_1 << 0x112233;

	CHAOS_MACHINE_XORRING64 gen_2;

	gen_2.set_time(4);
	gen_2.set_key({0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
	               0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
	               0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
	               0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
	               0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca});
	gen_2.set_space(1000);
	gen_2 << 0x112233;

	EXPECT_EQ(gen_1.pull(), gen_2.pull());
}

TEST(MACHINE, T__CHAOS_MACHINE__AdapterReset) {
	CHAOS_MACHINE_XORRING64 gen_1;

	gen_1.set_time(4);
	gen_1.set_space(1000);
	gen_1.set_key({0x84242f96eca9c41d, 0xa3c65b8776f96855, 0x5b34a39f070b5837,
	               0x4489affce4f31a1e, 0x2ffeeb0a48316f40, 0xdc2d9891fe68c022,
	               0x3659132bb12fea70, 0xaac17d8efa43cab8, 0xc4cb815590989b13,
	               0x5ee975283d71c93b, 0x691548c86c1bd540, 0x7910c41d10a1e6a5,
	               0x0b5fc64563b3e2a8, 0x047f7684e9fc949d, 0xb99181f2d8f685ca});
	gen_1.set_space(10);
	gen_1 << 0x112233;

	for(size_t i = 0; i < 1000; i++) gen_1.pull();
	uint64_t a = gen_1.pull();
	gen_1.reset();
	gen_1.set_space(100);

	EXPECT_NE(a, gen_1.pull());

	gen_1.reset();
	gen_1.set_space(10);
	gen_1 << 0x112233;
	for(size_t i = 0; i < 1000; i++) gen_1.pull();

	EXPECT_EQ(a, gen_1.pull());
}

TEST(MACHINE, T__CHAOS_MACHINE__AdapterAPI) {
	CHAOS_MACHINE_XORRING32 gen_1;
	CHAOS_MACHINE_XORRING32 gen_2;
	CHAOS_MACHINE_XORRING32 gen_3;
	CHAOS_MACHINE_XORRING32 gen_4;

	EXPECT_EQ(gen_1(), gen_2.pull());

	uint32_t tmp;
	gen_3 >> tmp;

	EXPECT_EQ(tmp, gen_4.pull());

	gen_1 << 0x11;
	gen_2.push(0x11);

	EXPECT_EQ(gen_1(), gen_2());
}
