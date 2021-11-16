#include <stdio.h>
#include "cmsis_os.h"

int main(int argc, const char* argv[])
{
	osKernelStart();

	sleep(1);
	uint32_t count = osKernelGetTickCount();
	CMSIS_IMPL_INFO("main: %d", count);
	return 0;
}
