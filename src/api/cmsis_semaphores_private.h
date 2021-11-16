#ifndef _CMSIS_SEMAPHORES_PRIVATE_H_
#define _CMSIS_SEMAPHORES_PRIVATE_H_

#include "cmsis_os.h"
#include "cmsis_autosar_os_queue.h"
#define AUTOSAR_OSSEM_HEAD_MAGICNO		0xDEADBEED
typedef struct {
  uint32_t			max_count;
  uint32_t			count;
  uint32_t			magicno;
  PosixOsQueueHeadType	waiting;
} CmsisSemType;

extern osStatus_t osSemaphoreAcquire_nolock(CmsisSemType *semp, uint32_t timeout);
extern osStatus_t osSemaphoreRelease_nolock(CmsisSemType *semp);


#endif /* _CMSIS_SEMAPHORES_PRIVATE_H_ */
