#include "autosar_os_ext_testfw.h"
#include "autosar_os_ext_user_config.h"
#include "autosar_os_ext_common_private.h"
#include <string.h>

void autosar_os_ext_testfw_setfunc(TaskType TaskID, void (*func) (void*))
{
  int i;
  for (i = 0; i < USER_THREAD_NUM; i++) {
    if (autosar_os_ext_testfw_funcmap[i].TaskID == TaskID) {
      autosar_os_ext_testfw_funcmap[i].func = func;
      break;
    }
  }
  return;
}

void autosar_os_ext_testfw_getfunc(void (**func) (void*))
{
  int i;
  TaskType TaskID;
  StatusType ercd = GetTaskID(&TaskID);
  if (ercd != E_OK) {
    *func = NULL;
  }
  for (i = 0; i < USER_THREAD_NUM; i++) {
    if (autosar_os_ext_testfw_funcmap[i].TaskID == TaskID) {
      *func = autosar_os_ext_testfw_funcmap[i].func;
    }
  }

  return;
}
void autosar_os_ext_testfw_clrfunc(void)
{
  int i;
  for (i = 0; i < USER_THREAD_NUM; i++) {
    autosar_os_ext_testfw_funcmap[i].func = NULL;
  }
  return;
}

static int current_testid = -1;
void autosar_os_ext_testfw_start_test(const char* name)
{
  int i;
  int len = strlen(name);
  for (i = 0; i < TEST_FW_OP_NUM; i++) {
    if (strlen(test_fw_operations[i].testname) != len) {
      continue;
    }
    if (!strncmp(test_fw_operations[i].testname, name, len)) {
      current_testid = i;
      syslog(LOG_NOTICE, "#TEST START: %s", name);
      return;
    }
  }
  current_testid = -1;
  return;
}

void autosar_os_ext_testfw_end_test(void)
{
  int exec_num;
  if ((current_testid < 0) || (current_testid >= TEST_FW_OP_NUM)) {
    syslog(LOG_NOTICE, "#TEST INVALID: unknown testid=%d", current_testid);
    return;
  }

  exec_num = test_fw_operations[current_testid].get_exec_num();
  if (test_fw_operations[current_testid].result == true) {
    syslog(LOG_NOTICE, "#TEST PASSED: %s. exec num: %d", test_fw_operations[current_testid].testname, exec_num);
  } else {
    syslog(LOG_NOTICE, "#TEST FAILED: %s. exec num: %d", test_fw_operations[current_testid].testname, exec_num);
  }
  return;
}
void autosar_os_ext_testfw_failed_callback(const char *testname, int testno)
{
  if ((current_testid < 0) || (current_testid >= TEST_FW_OP_NUM)) {
    return;
  }
  test_fw_operations[current_testid].result = false;
  return;
}

static void (*cyclic_handler_func) (void);
static void *cyclic_handler_argp;

void autosar_os_ext_testfw_set_cyclic_handler_func(void (*func) (void), void *argp)
{
  cyclic_handler_func = func;
  cyclic_handler_argp = argp;
  return;
}

void autosar_os_ext_testfw_clr_cyclic_handler_func(void)
{
  cyclic_handler_func = NULL;
  cyclic_handler_argp = NULL;
  return;
}
void *autosar_os_ext_testfw_get_cyclic_handler_argp(void)
{
  return cyclic_handler_argp;
}

ALARMCALLBACK(AutosarOsTestCyclicHandler)
{
  if (cyclic_handler_func != NULL) {
    cyclic_handler_func();
  }
  return;
}
