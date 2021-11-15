#include "cmsis_autosar_os_task_sync.h"
#include "autosar_os_ext_common_private.h"
#include "cmsis_os.h"

osStatus_t osDelay(uint32_t ticks)
{
  StatusType err;

  if (CurrentContextIsISR()) {
    return osErrorISR;
  }
  err = AutosarOsTaskSyncSleep(ticks);
  if (err != E_OK) {
    return osErrorISR;
  }
  return osOK;
}
