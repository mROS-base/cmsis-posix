#include <stdio.h>
#include "cmsis_os.h"
#include "autosar_os_ext_test.h"

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

	cmsis_thread_test_init();
	cmsis_thread_test_start();
	cmsis_thread_test_end();

	cmsis_kernel_test_init();
	cmsis_kernel_test_start();
	cmsis_kernel_test_end();

	cmsis_generic_test_init();
	cmsis_generic_test_start();
	cmsis_generic_test_end();

	cmsis_message_queue_test_init();
	cmsis_message_queue_test_start();
	cmsis_message_queue_test_end();
	return 0;
}
