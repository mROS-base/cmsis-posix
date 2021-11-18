#include <stdio.h>
#include "cmsis_os.h"
#include "autosar_os_ext_test.h"
#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	osKernelStart();
	return RUN_ALL_TESTS();
#if 0

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

	cmsis_semaphores_test_init();
	cmsis_semaphores_test_start();
	cmsis_semaphores_test_end();

	cmsis_mutexes_test_init();
	cmsis_mutexes_test_start();
	cmsis_mutexes_test_end();
	return 0;
#endif
}
