#include "cmsis_os.h"
#include "autosar_os_ext_test.h"
#include "autosar_os_ext_testfw.h"


#define TEST_MUTEXES_DELAY_TIME_10ms		10U
#define TEST_MUTEXES_DELAY_TIME_100ms		100U
#define TEST_MUTEXES_DELAY_TIME_110ms		110U
#define TEST_MUTEXES_DELAY_TIME_200ms		200U

#define TEST_ACQUIRE_H_TASK		1U
#define TEST_ACQUIRE_M_TASK		2U
#define TEST_ACQUIRE_L_TASK		3U
#define TEST_ACQUIRE_TASK_NUM	3U
static int test_acquire_task_waekup[TEST_ACQUIRE_TASK_NUM];
static int test_acquire_task_cunt = 0;


static void test_New_01(void);
static void test_Delete_01(void);
static void test_Delete_02(void);
static void test_Delete_03(void);
static void test_Acquire_01(void);
static void test_Acquire_02(void);
static void test_Acquire_03(void);
static void test_Acquire_04(void);
static void test_Acquire_05(void);
static void test_Acquire_06(void);
static void test_Release_01(void);
static void test_Release_02(void);
static void test_Release_03(void);

typedef struct {
    osMutexId_t id;
    char* api_name;
    int thread_id;
} TestAcquireInfoType;
#define MAX_THREAD_NUM  4U
static bool_t is_callback_activate[MAX_THREAD_NUM];

static void start_callback(bool_t* active_status)
{
    *active_status = true;
    return;
}

static void wait_for_end_callback(bool_t* active_status)
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
#define TESTNAME "cmsis_mutexes_test"

static int exec_cunt = 0U;

static void test_exec(void (*exec_test_func)(void))
{
    exec_test_func();
    exec_cunt++;
    return;
}

void cmsis_mutexes_test_init(void)
{
    autosar_os_ext_testfw_start_test(TESTNAME);
    return;
}
void cmsis_mutexes_test_end(void)
{
    autosar_os_ext_testfw_end_test();
    return;
}

void cmsis_mutexes_test_start(void)
{
    test_exec(test_New_01);

    test_exec(test_Delete_01);
    test_exec(test_Delete_02);
    test_exec(test_Delete_03);

    test_exec(test_Acquire_01);
    test_exec(test_Acquire_02);
    test_exec(test_Acquire_03);
    test_exec(test_Acquire_04);
    test_exec(test_Acquire_05);
    test_exec(test_Acquire_06);

    test_exec(test_Release_01);
    test_exec(test_Release_02);
    test_exec(test_Release_03);

    return;
}

int cmsis_mutexes_test_get_exec_num(void)
{
    return exec_cunt;
}

/*****************************************************************
 *
 * api test callback
 *
 *****************************************************************/

static void test_acquire_task_callback1(void* argp)
{
    osStatus_t err;
    TestAcquireInfoType* info;

    info = (TestAcquireInfoType*)argp;
    err = osMutexAcquire(info->id, 0);
    TestAssertEq(info->api_name, 1, osOK, err);

    (void)osDelay(TEST_MUTEXES_DELAY_TIME_100ms);

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}


static void test_acquire_task_callback2(void* argp)
{
    osStatus_t err;
    TestAcquireInfoType* info;

    info = (TestAcquireInfoType*)argp;
    err = osMutexAcquire(info->id, osWaitForever);
    TestAssertEq(info->api_name, 1, osOK, err);

    test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_H_TASK;
    test_acquire_task_cunt++;

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}

static void test_acquire_task_callback5(void* argp)
{
    osStatus_t err;
    TestAcquireInfoType* info;

    info = (TestAcquireInfoType*)argp;
    err = osMutexAcquire(info->id, TEST_MUTEXES_DELAY_TIME_200ms);
    TestAssertEq(info->api_name, 1, osOK, err);

    test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_H_TASK;
    test_acquire_task_cunt++;

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}


static void test_acquire_task_callback4(void* argp)
{
    osStatus_t err;
    TestAcquireInfoType* info;

    info = (TestAcquireInfoType*)argp;
    //printf("ack:callback4\n");
    err = osMutexAcquire(info->id, osWaitForever);
    TestAssertEq(info->api_name, 1, osOK, err);

    test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_H_TASK;
    test_acquire_task_cunt++;

    osDelay(100);

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);
    //printf("release:callback4\n");

    is_callback_activate[info->thread_id] = false;
    return;
}

static void test_acquire_task_callback3(void* argp)
{
    TestAcquireInfoType* info;
    TestAcquireInfoType info2;
    osStatus_t err;

    info = (TestAcquireInfoType*)argp;
    info2 = *info;
    info2.thread_id = 1;
    start_callback(&is_callback_activate[info2.thread_id]);
    (void)osThreadNew(test_acquire_task_callback4, (void*)&info2, NULL);

    //printf("ack:callback3\n");
    err = osMutexAcquire(info->id, osWaitForever);
    TestAssertEq(info->api_name, 1, osOK, err);

    osDelay(100);
    test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_H_TASK;
    test_acquire_task_cunt++;

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);
    //printf("release:callback3\n");

    wait_for_end_callback(&is_callback_activate[info2.thread_id]);
    is_callback_activate[info->thread_id] = false;
    return;
}

static void test_release_task_callback1(void* argp)
{
    osStatus_t err;
    TestAcquireInfoType* info;

    info = (TestAcquireInfoType*)argp;
    err = osMutexAcquire(info->id, osWaitForever);
    TestAssertEq(info->api_name, 1, osOK, err);

    err = osMutexRelease(info->id);
    TestAssertEq(info->api_name, 2, osOK, err);

    is_callback_activate[info->thread_id] = false;
    return;
}

/*****************************************************************
 *
 * api test main
 *
 *****************************************************************/
static void test_New_01(void)
{
    char* api_name = "osMutexNew:No.1";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre nothing
        //do
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //done
        err = osMutexDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

    }
    return;
}


static void test_Delete_01(void)
{
    char* api_name = "osMutexDelete:No.1";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //do
        err = osMutexDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //done
    }
    return;
}


static void test_Delete_02(void)
{
    char* api_name = "osMutexDelete:No.2";
    osMutexId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);
        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.api_name = "osMutexDelete:No.2:callback";
        info.id = id;
        info.thread_id = 0;

        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_release_task_callback1, (void*)&info, NULL);

        //do
        err = osMutexDelete(id);
        TestAssertEq(api_name, 3, osErrorResource, err);

        //done
        err = osMutexRelease(id);
        TestAssertEq(api_name, 4, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        err = osMutexDelete(id);
        TestAssertEq(api_name, 5, osOK, err);
    }
    return;
}


static void test_Delete_03(void)
{
    char* api_name = "osMutexDelete:No.3";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);
        err = osMutexDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osMutexDelete(id);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done
    }
    return;
}


static void test_Acquire_01(void)
{

    char* api_name = "osMutexAcquire:No.1";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        //do
        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //done

        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        err = osMutexDelete(id);
        TestAssertEq(api_name, 4, osOK, err);
    }

    return;
}

static void test_Acquire_02(void)
{

    char* api_name = "osMutexAcquire:No.2";
    osMutexId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osMutexAcquire:No.2:callback";
        info.id = id;
        info.thread_id = 0;

        info2 = info;
        info2.thread_id = 1;

        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);
        osDelay(10);
        (void)osThreadNew(test_release_task_callback1, (void*)&info2, NULL);

        //do
        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osErrorResource, err);

        //done
        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);

        err = osMutexDelete(id);
        TestAssertEq(api_name, 3, osOK, err);
    }

    return;
}

static void test_Acquire_03(void)
{

    char* api_name = "osMutexAcquire:No.3";
    osMutexId_t id;
    osStatus_t err;
    uint32_t start_time, end_time;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osMutexAcquire:No.3:callback";
        info.id = id;
        info.thread_id = 0;

        info2 = info;
        info2.thread_id = 1;

        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback1, (void*)&info, NULL);
        osDelay(10);
        (void)osThreadNew(test_release_task_callback1, (void*)&info2, NULL);

        //do
        start_time = osKernelGetTickCount();
        err = osMutexAcquire(id, TEST_MUTEXES_DELAY_TIME_10ms);
        end_time = osKernelGetTickCount();
        TestAssertInRange(api_name, 2, TEST_MUTEXES_DELAY_TIME_10ms, TEST_MUTEXES_DELAY_TIME_10ms + 1U, (end_time - start_time));
        TestAssertEq(api_name, 3, osErrorTimeoutResource, err);

        //done
        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);

        err = osMutexDelete(id);
        TestAssertEq(api_name, 4, osOK, err);
    }

    return;
}



static void test_Acquire_04(void)
{

    char* api_name = "osMutexAcquire:No.4";
    osMutexId_t id;
    osStatus_t err;
    TestAcquireInfoType info;
    TestAcquireInfoType info2;

    {
        //pre
        test_acquire_task_cunt = 0;
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osMutexAcquire:No.4:callback";
        info.id = id;
        info.thread_id = 0;

        info2 = info;
        info.thread_id = 1;

        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        //AutosarOsTimeIncTickCountSet((osWaitForever - 10U));

        start_callback(&is_callback_activate[0]);
        start_callback(&is_callback_activate[1]);
        (void)osThreadNew(test_acquire_task_callback5, (void*)&info, NULL);

        //do
        (void)osThreadNew(test_acquire_task_callback2, (void*)&info2, NULL);
        (void)osDelay(TEST_MUTEXES_DELAY_TIME_100ms);

        test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_L_TASK;
        test_acquire_task_cunt++;

        //done
        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
        TestAssertEq(api_name, 4, TEST_ACQUIRE_L_TASK, test_acquire_task_waekup[0]);
        TestAssertEq(api_name, 5, TEST_ACQUIRE_H_TASK, test_acquire_task_waekup[1]);
        TestAssertEq(api_name, 6, TEST_ACQUIRE_H_TASK, test_acquire_task_waekup[2]);

        err = osMutexDelete(id);
        TestAssertEq(api_name, 7, osOK, err);
    }

    return;
}



static void test_Acquire_05(void)
{

    char* api_name = "osMutexAcquire:No.5";
    osMutexId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        test_acquire_task_cunt = 0;
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        info.api_name = "osMutexAcquire:No.5:callback";
        info.id = id;
        info.thread_id = 0;


        //do
        //printf("ack:test\n");
        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);
        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_acquire_task_callback3, (void*)&info, NULL);

        osDelay(300);

        test_acquire_task_waekup[test_acquire_task_cunt] = TEST_ACQUIRE_L_TASK;
        test_acquire_task_cunt++;

        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osOK, err);
        //printf("release:test\n");

        wait_for_end_callback(&is_callback_activate[0]);
        wait_for_end_callback(&is_callback_activate[1]);
        TestAssertEq(api_name, 4, TEST_ACQUIRE_L_TASK, test_acquire_task_waekup[0U]);
        TestAssertEq(api_name, 5, TEST_ACQUIRE_H_TASK, test_acquire_task_waekup[1U]);
        TestAssertEq(api_name, 6, TEST_ACQUIRE_H_TASK, test_acquire_task_waekup[2U]);

        //done
        err = osMutexDelete(id);
        TestAssertEq(api_name, 7, osOK, err);
    }

    return;
}

static void test_Acquire_06(void)
{

    char* api_name = "osMutexAcquire:No.6";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);
        err = osMutexDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done
    }

    return;
}




static void test_Release_01(void)
{

    char* api_name = "osMutexRelease:No.1";
    osMutexId_t id;
    osStatus_t err;
    TestAcquireInfoType info;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);

        info.api_name = "osMutexRelease:No.1:callback";
        info.id = id;
        info.thread_id = 0;

        start_callback(&is_callback_activate[0]);
        (void)osThreadNew(test_release_task_callback1, (void*)&info, NULL);
        osDelay(100);

        //do
        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        //done
        wait_for_end_callback(&is_callback_activate[0]);
        err = osMutexDelete(id);
        TestAssertEq(api_name, 4, osOK, err);
    }

    return;
}

static void test_Release_02(void)
{

    char* api_name = "osMutexRelease:No.2";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osMutexAcquire(id, 0);
        TestAssertEq(api_name, 2, osOK, err);
        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osOK, err);

        //do
        err = osMutexRelease(id);
        TestAssertEq(api_name, 4, osErrorResource, err);

        //done
        err = osMutexDelete(id);
        TestAssertEq(api_name, 5, osOK, err);
    }

    return;
}

static void test_Release_03(void)
{

    char* api_name = "osMutexRelease:No.3";
    osMutexId_t id;
    osStatus_t err;

    {
        //pre
        id = osMutexNew(NULL);
        TestAssertNotEq(api_name, 1, id, NULL);

        err = osMutexDelete(id);
        TestAssertEq(api_name, 2, osOK, err);

        //do
        err = osMutexRelease(id);
        TestAssertEq(api_name, 3, osErrorParameter, err);

        //done

    }

    return;
}
