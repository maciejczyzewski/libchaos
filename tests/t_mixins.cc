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

	EXPECT_STREQ(a.c_str(), "4012BDD17547BBEE9A638E5A22829A7625F1B860");
	EXPECT_STREQ(b.c_str(), "D67B0DF02DEE6A4824083D004C722A3672734BCE");
	EXPECT_STREQ(c.c_str(), "4A2BC5F8117B83D16C69BA8F71F8D1CB2238ED94");
	EXPECT_STREQ(d.c_str(), "3F871BE85A214A0D4159675B992052699E5D8A46");
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

	EXPECT_STREQ(a.c_str(), "53C7B08FBA52042423F921D020DE30ACC93D3266");
	EXPECT_STREQ(b.c_str(), "ABC80FD2E489D828914F126F544A1FDCAD5CD806");
	EXPECT_STREQ(c.c_str(), "A97071E2BE1982C8D24EFBA3EC85C37B1F892BE9");
	EXPECT_STREQ(d.c_str(), "4B17D7A28566AE061E8F5A734D2650A3665D6903");
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

	EXPECT_STREQ(a.c_str(), "21504EE0E99E1073B191D591FC73D3A0D8C88454");
	EXPECT_STREQ(b.c_str(), "9A5E6500C880C490799024B0D36238DF72B2FCF8");
	EXPECT_STREQ(c.c_str(), "181DD01633E0743BB464606033F2E8259E09849E");
	EXPECT_STREQ(d.c_str(), "51387B0ED6AD2AE633D98C0927146587DD81440C");
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

	EXPECT_STREQ(a.c_str(), "C750FC104071A69944974036FD1B747C19643BB3");
	EXPECT_STREQ(b.c_str(), "8590CE2C93D6F06867080B0D2B40D466462A9E35");
	EXPECT_STREQ(c.c_str(), "39E71B96FCFCD037C2FFB6A8EA12EBF072AE0837");
	EXPECT_STREQ(d.c_str(), "600BF07C9F0040F6852B1D80C95D32C4B316D8F2");
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
