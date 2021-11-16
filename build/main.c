#include <stdio.h>
#include "cmsis_os.h"

static void test_thread_func(void* arg)
{
	CMSIS_IMPL_INFO("thread enter: %s", (const char*)arg);
	osThreadTerminate(NULL);
	CMSIS_IMPL_INFO("thread exit: %s", (const char*)arg);
	return;
}

int main(int argc, const char* argv[])
{
	osKernelStart();
	osThreadId_t id = osThreadNew(test_thread_func, "osThreadNew_Test01", NULL);
	CMSIS_IMPL_INFO("id=%p", id);

	CMSIS_IMPL_INFO("before");
	osStatus_t err = osDelay(1000);
	CMSIS_IMPL_INFO("after: %d", err);
	uint32_t count = osKernelGetTickCount();
	CMSIS_IMPL_INFO("main: %u", count);

	return 0;
}
