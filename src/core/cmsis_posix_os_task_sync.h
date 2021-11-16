#ifndef _CMSIS_POSIX_OS_TASK_SYNC_H_
#define _CMSIS_POSIX_OS_TASK_SYNC_H_

#include "cmsis_posix_os_queue.h"
#include "cmsis_posix_os_types.h"
#include "cmsis_os.h"

extern osStatus_t PosixOsThreadSyncWait(uint32_t timeout);

extern void *PosixOsTaskSyncWait(PosixOsQueueHeadType *waiting_queue, uint32_t timeout, osStatus_t*ercdp);
extern bool_t PosixOsTaskSyncWakeupFirstEntry(PosixOsQueueHeadType *waiting_queue, void *data, osStatus_t ercd);

extern void PosixOsThreadSyncInit(void);
extern void PosixOsThreadSyncLock(void);
extern void PosixOsThreadSyncUnlock(void);

#endif /* _CMSIS_POSIX_OS_TASK_SYNC_H_ */
