#ifndef _CMSIS_AUTOSAR_OS_TASK_SYNC_H_
#define _CMSIS_AUTOSAR_OS_TASK_SYNC_H_

#include "cmsis_autosar_os_queue.h"
#include "cmsis_autosar_os_types.h"

extern StatusType AutosarOsTaskSyncSleep(uint32_t timeout);
extern StatusType AutosarOsTaskSyncWakeup(TaskType taskID, StatusType ercd);

extern void *AutosarOsTaskSyncWait(AutosarOsQueueHeadType *waiting_queue, uint32_t timeout, StatusType *ercdp, TaskType taskID);
extern bool_t AutosarOsTaskSyncWakeupFirstEntry(AutosarOsQueueHeadType *waiting_queue, void *data, StatusType ercd);

#endif /* _CMSIS_AUTOSAR_OS_TASK_SYNC_H_ */
