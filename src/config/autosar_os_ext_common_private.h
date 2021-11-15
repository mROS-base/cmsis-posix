#ifndef _AUTOSAR_OS_EXTEND_COMMON_PRIVATE_H_
#define _AUTOSAR_OS_EXTEND_COMMON_PRIVATE_H_

#include "autosar_os_ext_common.h"

#ifdef OS_AUTOSAR

#else

extern StatusType GetTaskID(TaskRefType TaskID);
extern StatusType SetEvent(TaskType TaskID, EventMaskType Mask);
extern StatusType ClearEvent(EventMaskType Mask);
extern StatusType WaitEvent(EventMaskType Mask);
extern void ResumeAllInterrupts(void);
extern void SuspendAllInterrupts(void);
extern void ResumeOSInterrupts(void);
extern void SuspendOSInterrupts(void);
extern StatusType ActivateTask(TaskType TaskID);
extern StatusType TerminateTask(void);

#endif /* OS_AUTOSAR */

extern bool_t CurrentContextIsISR(void);



#endif /* _AUTOSAR_OS_EXTEND_COMMON_H_ */
