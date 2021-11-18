#include "cmsis_os.h"
#include "autosar_os_ext_test.h"
#include "autosar_os_ext_testfw.h"


static void test_gettick_01(void);
//static void test_gettick_02(void);
//static void test_gettick_callback1(void);

#define TEST_KERNEL_SLEEP_TIME_100ms		100U
#define TEST_KERNEL_SLEEP_TIME_200ms		200U
#define TEST_KERNEL_SLEEP_TIME_1000ms		1000U
#define TEST_KERNEL_SLEEP_TIME_2000ms		2000U

static int exec_cunt = 0U;

static void test_exec(void (*exec_test_func)(void))
{
    exec_test_func();
    exec_cunt++;
    return;
}

#define TESTNAME "cmsis_kernel_test"
void cmsis_kernel_test_init(void)
{
    autosar_os_ext_testfw_start_test(TESTNAME);
    return;
}
void cmsis_kernel_test_end(void)
{
    autosar_os_ext_testfw_end_test();
    return;
}

void cmsis_kernel_test_start(void)
{
    test_exec(test_gettick_01);
    //test_exec(test_gettick_02);
    return;
}

int cmsis_kernel_test_get_exec_num(void)
{
    return exec_cunt;
}

#if 0
static int test_kernel_callback_waekup_count = 0;
static void test_gettick_callback1(void)
{
    char* api_name = "osKernelGetTickCount:No.2";
    uint32_t wakeup_time;

    {
        //osKernelGetTickCount:No.1 100ms + osKernelGetTickCount:No.2 100ms = 200ms
        if (test_kernel_callback_waekup_count < TEST_KERNEL_SLEEP_TIME_100ms) {
            test_kernel_callback_waekup_count++;
        }
        else if (test_kernel_callback_waekup_count == TEST_KERNEL_SLEEP_TIME_100ms) {
            wakeup_time = osKernelGetTickCount();
            TestAssertInRange(api_name, 1, wakeup_time, wakeup_time + 1U, TEST_KERNEL_SLEEP_TIME_200ms);

            test_kernel_callback_waekup_count++;
        }

    }

    return;
}
#endif

static void test_gettick_01(void)
{
    char* api_name = "osKernelGetTickCount:No.1";
    uint32_t before_time;
    uint32_t wakeup_time;
    uint32_t slp_time;

    {
        before_time = osKernelGetTickCount();
        (void)osDelay(TEST_KERNEL_SLEEP_TIME_100ms);
        wakeup_time = osKernelGetTickCount();

        slp_time = wakeup_time - before_time;
        TestAssertInRange(api_name, 1, TEST_KERNEL_SLEEP_TIME_100ms, TEST_KERNEL_SLEEP_TIME_100ms + 1U, slp_time);

    }

    return;
}

#if 0
static void test_gettick_02(void)
{
    {
        autosar_os_ext_testfw_set_cyclic_handler_func(test_gettick_callback1, NULL);
        (void)osDelay(TEST_KERNEL_SLEEP_TIME_200ms);

        autosar_os_ext_testfw_clrfunc();
    }

    return;
}


#endif
