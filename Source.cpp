#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include "CompileTime.h"

int main()
{
	printf("CompileTime random number is %lu\n",(uint32_t)(CompileTimeRandom(0,UINT32_MAX)));
	printf("CompileTime random number is %lu\n",(uint32_t)(CompileTimeRandom(0,UINT32_MAX)));
}