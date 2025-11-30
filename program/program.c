#include <stdio.h>
#include <stdint.h>

extern uint64_t FUNC(uint64_t x, uint64_t y, uint64_t z, uint64_t w);

int main() {
	uint64_t x = 100;
	uint64_t y = 200;
	uint64_t z = 300;
	uint64_t w = 400;

	uint64_t result = FUNC(x, y, z, w);

	printf("OUTPUT=%llu\n", result);
}
