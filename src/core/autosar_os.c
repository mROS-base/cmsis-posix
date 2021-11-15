#include "autosar_os_ext_common.h"
#include "autosar_os_ext_common_private.h"

static uint8_t      sus_all_cnt = 0U;
static uint8_t      sus_os_cnt = 0U;

StatusType GetTaskID(TaskRefType TaskID)
{
    //TODO
  return E_OK;
}
StatusType ActivateTask(TaskType TaskID)
{
    //TODO
  return E_OK;
}
StatusType TerminateTask(void)
{
    //TODO
    return E_OK;
}

StatusType SetEvent(TaskType TaskID, EventMaskType Mask)
{
    //TODO

  return E_OK;
}

StatusType ClearEvent(EventMaskType Mask)
{
    //TODO

  return E_OK;
}

StatusType WaitEvent(EventMaskType Mask)
{
    //TODO

  return E_OK;
}
void ResumeAllInterrupts(void)
{
  sus_all_cnt--;
  if (sus_all_cnt == 0) {
      //TODO
  }
  return;
}

void SuspendAllInterrupts(void)
{
  if (sus_all_cnt == 0) {
      //TODO
  }
  sus_all_cnt++;
  return;
}

void ResumeOSInterrupts(void)
{
  sus_os_cnt--;
  if (sus_os_cnt == 0) {
      //TODO
  }
  return;
}

void SuspendOSInterrupts(void)
{
  if (sus_os_cnt == 0) {
      //TODO
  }
  sus_os_cnt++;
  return;
}

ALARMCALLBACK(AutosarOsExtCyclicHandler)
{
    //TODO
    return;
}

bool_t CurrentContextIsISR(void)
{
  return true;//TODO
}
