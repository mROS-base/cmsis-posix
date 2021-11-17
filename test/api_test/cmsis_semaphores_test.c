#include "posix_os_ext_common_private.h"
#include "cmsis_os.h"
#include "autosar_os_ext_test.h"
#include "autosar_os_ext_testfw.h"

#define CMSIS_SEMPHORES_TEST_TRUE   1
#define CMSIS_SEMPHORES_TEST_FALSE  0

#define TEST_SEMPHORES_MAX_TOKEN1	1U
#define TEST_SEMPHORES_INIT_TOKEN1	1U
#define TEST_SEMPHORES_MAX_TOKEN2	2U
#define TEST_SEMPHORES_INIT_TOKEN2	2U

#define TEST_SEMPHORES_DELAY_TIME_10ms 10U
#define TEST_SEMPHORES_DELAY_TIME_100ms 100U
#define TEST_SEMPHORES_DELAY_TIME_110ms 110U

typedef struct {
    int thread_id;
    osSemaphoreId_t id;
    char* api_name;
    uint32_t timeout;
} TestAcquireInfoType;

static void test_New_01(void);
static void test_Acquire_01(void);
static void test_Acquire_02(void);
static void test_Acquire_03(void);
static void test_Acquire_04(void);
static void test_Acquire_05(void);
static void test_Acquire_06(void);
static void test_Acquire_07(void);
static void test_Acquire_08(void);
//static void test_Acquire_09(void);
static void test_Release_01(void);
static void test_Release_02(void);
static void test_Release_03(void);
//static void test_Release_04(void);
static void test_Release_05(void);
static void test_Release_06(void);
//static void test_Release_07(void);
static void test_dleate_01(void);
static void test_dleate_02(void);

static int exec_cunt = 0U;

static void test_exec(void (*exec_test_func)(void))
{
    exec_test_func();
    exec_cunt++;
    return;
}

#define TESTNAME "cmsis_semaphores_test"
void cmsis_semaphores_test_init(void)
{
    autosar_os_ext_testfw_start_test(TESTNAME);
    return;
}
void cmsis_semaphores_test_end(void)
{
    autosar_os_ext_testfw_end_test();
    return;
}

void cmsis_semaphores_test_start(void)
{
    test_exec(test_New_01);

    test_exec(test_Acquire_01);
    test_exec(test_Acquire_02);
    test_exec(test_Acquire_03);
    test_exec(test_Acquire_04);
    test_exec(test_Acquire_05);
    test_exec(test_Acquire_06);
    test_exec(test_Acquire_07);
    test_exec(test_Acquire_08);
    //test_exec(test_Acquire_09);

    test_exec(test_Release_01);
    test_exec(test_Release_02);
    test_exec(test_Release_03);
#if 0
    //test_exec(test_Release_04);
    //(void)osDelay(300);
#endif
    test_exec(test_Release_05);
    test_exec(test_Release_06);

    //test_exec(test_Release_07);

    test_exec(test_dleate_01);
    test_exec(test_dleate_02);

    return;
}

int cmsis_semaphores_test_get_exec_num(void)
{
    return exec_cunt;
}

static void test_New_01(void)
{
    char* api_name = "osSemaphoreNew:No.1";
    osSemaphoreId_t id;
    osStatus_t err;
    {
        //pre nothing

        //do
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);
    }

    return;
}

static void test_Acquire_01(void)
{
    char* api_name = "osSemaphoreAcquire:No.1";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //do
        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //done
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

    }

    return;
}
#define MAX_THREAD_NUM  4U
static bool_t is_callback_activate[MAX_THREAD_NUM];
static void test_acquire_task_callback1(void* argp)
{
    TestAcquireInfoType* info;
    osStatus_t err;

    info = (TestAcquireInfoType*)argp;
    err = osSemaphoreAcquire(info->id, 0);
    TestAssertEq(info->api_name, 1, osOK, err);

    (void)osDelay(info->timeout);

    err = osSemaphoreRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}

static void test_acquire_task_callback2(void* argp)
{
    TestAcquireInfoType* info;
    osStatus_t err;

    info = (TestAcquireInfoType*)argp;
    err = osSemaphoreAcquire(info->id, osWaitForever);
    TestAssertEq(info->api_name, 1, osOK, err);

    err = osSemaphoreRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}

static void test_release_task_callback1(void* argp)
{
    TestAcquireInfoType* info;
    osStatus_t err;

    info = (TestAcquireInfoType*)argp;
    err = osSemaphoreRelease(info->id);
    TestAssertEq(info->api_name, 1, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}
static void start_callback(bool_t* active_status)
{
    *active_status = true;
    return;
}

static void wait_for_end_callback(bool_t *active_status)
{
    while (true) {
        if ((*active_status) == false) {
            break;
        }
        else {
            osDelay(100);
        }
    }
    return;
}

static void test_Acquire_02(void)
{
    char* api_name = "osSemaphoreAcquire:No.2";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.2:callback";
        info.id = id;
        info.timeout = TEST_SEMPHORES_DELAY_TIME_100ms;
        info.thread_id = 0;
        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);

        //do
        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_110ms);

        //done
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
    }

    return;
}

static void test_Acquire_03(void)
{
    char* api_name = "osSemaphoreAcquire:No.3";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.3:callback";
        info.id = id;
        info.timeout = TEST_SEMPHORES_DELAY_TIME_100ms;
        info.thread_id = 0;

        info2 = info;
        info2.thread_id = 1;

        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info2, NULL);


        (void)osDelay(10);

        //do
        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osErrorResource, err);

        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_110ms);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 3, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
    }

    return;
}

static void test_Acquire_04(void)
{
    char* api_name = "osSemaphoreAcquire:No.4";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.4:callback";
        info.id = id;
        info.timeout = TEST_SEMPHORES_DELAY_TIME_100ms;
        info.thread_id = 0;

        info2 = info;
        info2.thread_id = 1;

        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info2, NULL);

        (void)osDelay(10);

        //do
        err = osSemaphoreAcquire(id, TEST_SEMPHORES_DELAY_TIME_10ms);
        TestAssertEq(api_name, 2, osErrorTimeoutResource, err);

        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_110ms);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 3, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
    }

    return;
}


static void test_Acquire_05(void)
{
    char* api_name = "osSemaphoreAcquire:No.5";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.5:callback";
        info.id = id;
        info.thread_id = 0;

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info, NULL);

        //AutosarOsTimeIncTickCountSet((osWaitForever - 10U));
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_100ms);

        //done
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        (void)osDelay(100);

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
    }

    return;
}

static void test_Acquire_06(void)
{
    char* api_name = "osSemaphoreAcquire:No.6";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.6:callback";
        info.id = id;
        info.thread_id = 0;

        info2 = info;
        info2.thread_id = 1;

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info, NULL);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info2, NULL);


        //AutosarOsTimeIncTickCountSet((osWaitForever - 10U));
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_100ms);

        //done
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        (void)osDelay(100);

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
    }

    return;
}

static void test_Acquire_07(void)
{
    char* api_name = "osSemaphoreAcquire:No.7";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;
    TestAcquireInfoType info3;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osSemaphoreAcquire:No.7:callback";
        info.id = id;
        info.thread_id = 0;
        info2 = info;
        info3 = info;
        info2.thread_id = 1;
        info3.thread_id = 2;
        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        start_callback(&is_callback_activate[2]);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info, NULL);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info2, NULL);
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info3, NULL);


        //AutosarOsTimeIncTickCountSet((osWaitForever - 10U));
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_100ms);

        //done
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        (void)osDelay(100);

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
        wait_for_end_callback(&is_callback_activate[2]);
    }

    return;
}

static void test_Acquire_08(void)
{
    char* api_name = "osSemaphoreAcquire:No.8";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done

    }

    return;
}

#if 0
static TestAcquireInfoType callback_info;
static int is_first_callback;
static void test_acquire_task_callback3(void)
{

    osStatus_t err;

    if (is_first_callback == CMSIS_SEMPHORES_TEST_TRUE) {
        err = osSemaphoreAcquire(callback_info.id, 0);
        TestAssertEq(callback_info.api_name, 1, osOK, err);

        err = osSemaphoreRelease(callback_info.id);
        TestAssertEq(callback_info.api_name, 2, osOK, err);
        is_first_callback = CMSIS_SEMPHORES_TEST_FALSE;
    }

    return;
}
#endif
#if 0
static void test_Acquire_09(void)
{
    char* api_name = "osSemaphoreAcquire:No.9";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        is_first_callback = CMSIS_SEMPHORES_TEST_TRUE;
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);
        callback_info.id = id;
        callback_info.api_name = "osSemaphoreAcquire:No.9:callback";

        //do
        autosar_os_ext_testfw_set_cyclic_handler_func(test_acquire_task_callback3, NULL);
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_10ms);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

    }

    return;
}
#endif

static void test_Release_01(void)
{
    char* api_name = "osSemaphoreRelease:No.1";
    osSemaphoreId_t id;
    TestAcquireInfoType info;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.id = id;
        info.api_name = "osSemaphoreRelease:No.1:callback";
        info.timeout = TEST_SEMPHORES_DELAY_TIME_100ms;
        info.thread_id = 0;

        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);

        //do
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

    }

    return;
}

static void test_Release_02(void)
{
    char* api_name = "osSemaphoreRelease:No.2";
    osSemaphoreId_t id;
    TestAcquireInfoType info;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.id = id;
        info.api_name = "osSemaphoreRelease:No.2:callback";
        info.timeout = TEST_SEMPHORES_DELAY_TIME_10ms;
        info.thread_id = 0;

        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);

        wait_for_end_callback(&is_callback_activate[0]);

        //do
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);


        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 4, osOK, err);

    }

    return;
}

static void test_Release_03(void)
{
    char* api_name = "osSemaphoreRelease:No.3";
    osSemaphoreId_t id;
    TestAcquireInfoType info;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.id = id;
        info.api_name = "osSemaphoreRelease:No.3:callback";
        info.timeout = TEST_SEMPHORES_DELAY_TIME_10ms;
        info.thread_id = 0;

        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);

        wait_for_end_callback(&is_callback_activate[0]);

        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        //do
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 4, osErrorResource, err);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 5, osOK, err);

    }

    return;
}

#if 0
static void test_Release_04(void)
{
    char* api_name = "osSemaphoreRelease:No.4";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        is_first_callback = CMSIS_SEMPHORES_TEST_TRUE;
        callback_info.id = id;
        callback_info.api_name = "osSemaphoreRelease:No.4:callback";

        //do
        autosar_os_ext_testfw_set_cyclic_handler_func(test_acquire_task_callback3, NULL);
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_10ms);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 3, osOK, err);

    }

    return;
}
#endif

static void test_Release_05(void)
{
    char* api_name = "osSemaphoreRelease:No.5";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done

    }

    return;
}


static void test_Release_06(void)
{
    char* api_name = "osSemaphoreRelease:No.6";
    osSemaphoreId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.id = id;
        info.api_name = "osSemaphoreRelease:No.6:callback";
        info.thread_id = 0;

        //do
        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_release_task_callback1, (void*)&info, NULL);
        wait_for_end_callback(&is_callback_activate[0]);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 3, osOK, err);
        err = osSemaphoreRelease(id);
        TestAssertEq(api_name, 4, osOK, err);

        //done

        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 5, osOK, err);
    }

    return;
}


#if 0
static void test_release_task_callback2(void)
{

    osStatus_t err;

    if (is_first_callback == CMSIS_SEMPHORES_TEST_TRUE) {
        err = osSemaphoreRelease(callback_info.id);
        TestAssertEq(callback_info.api_name, 1, osOK, err);
        is_first_callback = CMSIS_SEMPHORES_TEST_FALSE;
    }

    return;
}
static void test_Release_07(void)
{
    char* api_name = "osSemaphoreRelease:No.7";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        is_first_callback = CMSIS_SEMPHORES_TEST_TRUE;
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN2, TEST_SEMPHORES_INIT_TOKEN2, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osSemaphoreAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        callback_info.id = id;
        callback_info.api_name = "osSemaphoreRelease:No.7:callback";

        //do
        autosar_os_ext_testfw_set_cyclic_handler_func(test_release_task_callback2, NULL);
        (void)osDelay(TEST_SEMPHORES_DELAY_TIME_10ms);

        //done
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 3, osOK, err);

    }

    return;
}
#endif

static void test_dleate_01(void)
{
    char* api_name = "osSemaphoreDleate:No.1";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //do
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //done

    }

    return;
}

static void test_dleate_02(void)
{
    char* api_name = "osSemaphoreDleate:No.2";
    osSemaphoreId_t id;
    osStatus_t err;

    {
        //pre
        id = osSemaphoreNew(TEST_SEMPHORES_MAX_TOKEN1, TEST_SEMPHORES_INIT_TOKEN1, NULL);
        TestAssertNotEq(api_name, 1, id, NULL);
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osSemaphoreDelete(id);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done

    }

    return;
}
