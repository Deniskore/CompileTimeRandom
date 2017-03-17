#include <cstdint>
#include <stdio.h>
#include "CompileTime.h"

int main()
{
	constexpr uint32_t random = rValue;
	constexpr uint64_t random64 = rValue64;
	
	uint32_t first = random;
	uint64_t second = random64;

	printf("%lu\n", first);
	printf("%llu", second);
}