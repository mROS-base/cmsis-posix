#include "cmsis_os.h"
#include "autosar_os_ext_test.h"
#include "autosar_os_ext_testfw.h"


static void test_thread_create_01(void);
static void test_thread_new_01(void);
static void test_thread_term_01(void);

static int exec_cunt = 0U;

static void test_exec(void (*exec_test_func)(void))
{
  exec_test_func();
  exec_cunt++;
  return;
}

#define TESTNAME "cmsis_thread_test"
void cmsis_thread_test_init(void)
{
  autosar_os_ext_testfw_start_test(TESTNAME);
  return;
}
void cmsis_thread_test_end(void)
{
  autosar_os_ext_testfw_end_test();
  return;
}

void cmsis_thread_test_start(void)
{
  test_exec(test_thread_create_01);
  test_exec(test_thread_new_01);
  test_exec(test_thread_term_01);
  return;
}


int cmsis_thread_test_get_exec_num(void)
{
  return exec_cunt;
}

static void test_thread_task_callback1(void *argument)
{
  char *msg = (char*)argument;
  TestAssertStringEq("osThreadCreate:callback1", 2, "osThreadCreate_Test01", msg);
  return;
}

static void test_thread_create_01(void)
{
  osThreadDef_t thrdef;

  thrdef.pthread = (os_pthread)test_thread_task_callback1;

  osThreadId id = osThreadCreate(&thrdef, "osThreadCreate_Test01");
  TestAssertNotEq("osThreadCreate:No.1", 1, NULL, id);
  osDelay(10);
  return;
}

static void test_thread_task_callback2(void *argument)
{
  char *msg = (char*)argument;
  TestAssertStringEq("osThreadNew:callback1", 2, "osThreadNew_Test01", msg);
  return;
}

static void test_thread_new_01(void)
{
  osThreadId_t id = osThreadNew(test_thread_task_callback2, "osThreadNew_Test01", NULL);
  TestAssertNotEq("osThreadNew:No.1", 1, NULL, id);
  osDelay(10);
  return;
}

static int test_thread_task_callback3_terminate_check = 0;
static void test_thread_task_callback3(void *argument)
{
  char *msg = (char*)argument;
  TestAssertStringEq("osThreadNew:callback1", 2, "osThreadTerminate_Test01", msg);
  (void)osThreadTerminate(NULL);
  test_thread_task_callback3_terminate_check = 1;
  return;
}


static void test_thread_term_01(void)
{
  osThreadId_t id = osThreadNew(test_thread_task_callback3, "osThreadTerminate_Test01", NULL);
  TestAssertNotEq("osThreadTerminate:No.1", 1, NULL, id);
  osDelay(10);

  TestAssertEq("osThreadTerminate:No.1", 3, 0, test_thread_task_callback3_terminate_check);
  return;
}
