#ifndef _CMSIS_AUTOSAR_OS_TASK_SYNC_H_
#define _CMSIS_AUTOSAR_OS_TASK_SYNC_H_

#include "cmsis_autosar_os_queue.h"
#include "cmsis_autosar_os_types.h"
#include "cmsis_os.h"

extern osStatus_t PosixOsThreadSyncWait(uint32_t timeout);

extern void *PosixOsTaskSyncWait(PosixOsQueueHeadType *waiting_queue, uint32_t timeout, StatusType *ercdp, TaskType taskID);
extern bool_t PosixOsTaskSyncWakeupFirstEntry(PosixOsQueueHeadType *waiting_queue, void *data, StatusType ercd);

extern void PosixOsThreadSyncInit(void);
extern void PosixOsThreadSyncLock(void);
extern void PosixOsThreadSyncUnlock(void);

#endif /* _CMSIS_AUTOSAR_OS_TASK_SYNC_H_ */
