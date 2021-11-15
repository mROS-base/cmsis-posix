#ifndef _AUTOSAR_OS_EXTEND_COMMON_H_
#define _AUTOSAR_OS_EXTEND_COMMON_H_


#include "cmsis_impl.h"

typedef int StatusType;
typedef int AlarmType;
typedef int* TaskType;
typedef TaskType* TaskRefType;
typedef int AlarmType;
typedef int EventMaskType;

#define E_OK	0
#define E_OS_CALLEVEL	2
#define E_OS_ID 3
#define E_OS_LIMIT 4
#define E_OS_RESOURCE 6

#define AutosarOsExtEvent	0x00000001

#define TASK(func)	void TaskMain ## func (intptr_t exinf)
#define ALARMCALLBACK(func) void AlarmManin ## func (intptr_t exinf)

#endif /* _AUTOSAR_OS_EXTEND_COMMON_H_ */
