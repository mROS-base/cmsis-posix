#include "autosar_os_ext_sys_config.h"
#include "autosar_os_ext_common_private.h"
#include "cmsis_user_config.h"

StatusType AutosarOsTaskConfigSet(const osThreadFunc_t func, void *argument)
{
  int i;
  int user_thread_num;
  UserThreadAutosarOsTaskMapEntryType* map_p;

  user_thread_num = cmsis_get_user_thread_num();
  map_p = cmsis_get_user_thread_map();

  for (i = 0; i < user_thread_num; i++) {
    if (map_p[i].func == func) {
      map_p[i].argument = argument;
      return E_OK;
    }
  }
  return E_OS_ID;
}

StatusType AutosarOsTaskConfigGetTaskID(const osThreadFunc_t func, TaskType *taskID)
{
  int i;
  int user_thread_num;
  UserThreadAutosarOsTaskMapEntryType* map_p;

  user_thread_num = cmsis_get_user_thread_num();
  map_p = cmsis_get_user_thread_map();

  for (i = 0; i < user_thread_num; i++) {
    if (map_p[i].func == func) {
      *taskID = map_p[i].taskID;
      return E_OK;
    }
  }
  return E_OS_ID;
}

StatusType AutosarOsTaskConfigGet(UserThreadAutosarOsTaskMapEntryType *config)
{
  int i;
  TaskType taskID;
  StatusType ercd;
  int user_thread_num;
  UserThreadAutosarOsTaskMapEntryType* map_p;

  user_thread_num = cmsis_get_user_thread_num();
  map_p = cmsis_get_user_thread_map();

  ercd = GetTaskID(&taskID);
  if (ercd != E_OK) {
    return ercd;
  }

  for (i = 0; i < user_thread_num; i++) {
    if (map_p[i].taskID == taskID) {
      *config = map_p[i];
      return E_OK;
    }
  }
  return E_OS_ID;
}
