#include "gtest/gtest.h"

#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <chaos.h>

/// mixins /////////////////////////////////////////////////////////////////////

TEST(MIXINS, T__CHAOS_TRUELY__AdapterInitialization) {
	chaos::truely<CHAOS_MACHINE_NCG, std::random_device> gen_0, gen_1;

	size_t n = 100;
	std::vector<CHAOS_MACHINE_NCG::size_pull> a(n), b(n);

	for (size_t i = 0; i < n; i++)
		a.push_back(gen_0());

	for (size_t i = 0; i < n; i++)
		b.push_back(gen_1());

	EXPECT_NE(a, b);
}

TEST(MIXINS, T__CHAOS_PASSWORD__AvalancheTime) {
	std::string a, b, c, d;

	a = chaos::password<CHAOS_MACHINE_XORRING32, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF");
	b = chaos::password<CHAOS_MACHINE_XORRING32, 31, 10, 40>("OMG AAA OMG1",
	                                                         "WTF");
	c = chaos::password<CHAOS_MACHINE_XORRING32, 32, 10, 40>("OMG AAA OMG1",
	                                                         "WTF");
	d = chaos::password<CHAOS_MACHINE_XORRING32, 33, 10, 40>("OMG AAA OMG1",
	                                                         "WTF");

	EXPECT_STREQ(a.c_str(), "74C9B4DE6AB8BA70F81EA64442729674E6CC6BC8");
	EXPECT_STREQ(b.c_str(), "9F64E0E63A39CD4D21E89896C95290FC56F9C0A8");
	EXPECT_STREQ(c.c_str(), "64699434AC8CA4C01586906217B07B2429FAE617");
	EXPECT_STREQ(d.c_str(), "DF7FC0FC8E0C43FA7A1BFB16D95DB012161A5C50");
}

TEST(MIXINS, T__CHAOS_PASSWORD__AvalancheSpace) {
	std::string a, b, c, d;

	a = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTFX");
	b = chaos::password<CHAOS_MACHINE_XORRING64, 30, 11, 40>("OMG AAA OMG1",
	                                                         "WTFX");
	c = chaos::password<CHAOS_MACHINE_XORRING64, 30, 12, 40>("OMG AAA OMG1",
	                                                         "WTFX");
	d = chaos::password<CHAOS_MACHINE_XORRING64, 30, 13, 40>("OMG AAA OMG1",
	                                                         "WTFX");

	EXPECT_STREQ(a.c_str(), "9E940EA08CB35E2057A05EFE34797545E15001F0");
	EXPECT_STREQ(b.c_str(), "47FD2900ABED1F151917C640AAA52AF8A5EA3606");
	EXPECT_STREQ(c.c_str(), "F8E8768413D07DBD4C08320EA4594A00AE90C1F0");
	EXPECT_STREQ(d.c_str(), "3E5D290055E81F15F449464044042AF8119DB606");
}

TEST(MIXINS, T__CHAOS_PASSWORD__AvalanchePassword) {
	std::string a, b, c, d;

	a = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF");
	b = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG2",
	                                                         "WTF");
	c = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG ",
	                                                         "WTF");
	d = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("2MG AAA OMG1",
	                                                         "WTF");

	EXPECT_STREQ(a.c_str(), "D437F6008DE3C755D8162620EEF4B1CB17814A00");
	EXPECT_STREQ(b.c_str(), "4C484009F951E30033DD4ECEACD817308EC279A2");
	EXPECT_STREQ(c.c_str(), "875C515D55AC265891CFBE7C9407DA58C2691D64");
	EXPECT_STREQ(d.c_str(), "6757C8B0B2C2DCB8986E5ADA9E6629837D70F463");
}

TEST(MIXINS, T__CHAOS_PASSWORD__AvalancheSalt) {
	std::string a, b, c, d;

	a = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF1");
	b = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF2");
	c = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF3");
	d = chaos::password<CHAOS_MACHINE_XORRING64, 30, 10, 40>("OMG AAA OMG1",
	                                                         "WTF4");

	EXPECT_STREQ(a.c_str(), "C67A0774E79BCBF84B2FCBAFC7C4F4057B8B045E");
	EXPECT_STREQ(b.c_str(), "C10EF36C86C0B24081C6362E958EFA4FE319E872");
	EXPECT_STREQ(c.c_str(), "A4C6D72484127BED7E9D2E76B025938DC808D380");
	EXPECT_STREQ(d.c_str(), "9766FDEE71EAFD90AD8304E01DC174A3F2285176");
}

/* FIXME
TEST(MIXINS, MACROS) {
  // machines
  EXPECT_EQ(CHAOS_META_NAME(CHAOS_MACHINE_NCG),
            "NCG (Naive Czyzewski Generator)");
  EXPECT_EQ(CHAOS_META_AUTHORS(CHAOS_MACHINE_NCG), "Maciej A. Czyzewski");

  // prngs
  EXPECT_EQ(CHAOS_META_NAME(CHAOS_PRNG_KISS), "KISS (Keep It Simple Stupid)");
  EXPECT_EQ(CHAOS_META_AUTHORS(CHAOS_PRNG_KISS), "G. Marsaglia and A. Zaman");
}
*/
