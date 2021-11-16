#include <stdio.h>
#include "cmsis_os.h"

int main(int argc, const char* argv[])
{
	osKernelStart();

	CMSIS_IMPL_INFO("before");
	osStatus_t err = osDelay(2);
	CMSIS_IMPL_INFO("after: %d", err);
	uint32_t count = osKernelGetTickCount();
	CMSIS_IMPL_INFO("main: %u", count);
	return 0;
}
