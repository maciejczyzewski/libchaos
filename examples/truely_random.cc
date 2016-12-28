#include <chaos.h>

chaos::truely<CHAOS_MACHINE_XORRING64, std::random_device> gen;

int main(void) {
	for (size_t i = 0; i < 30; i += 3)
		printf("%p\t%p\t\%p\n", gen(), gen(), gen());
}
