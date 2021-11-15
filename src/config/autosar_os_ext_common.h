#ifndef _AUTOSAR_OS_EXTEND_COMMON_H_
#define _AUTOSAR_OS_EXTEND_COMMON_H_


#ifdef OS_AUTOSAR
#include "kernel/kernel_impl.h"
#include "Os.h"
#include "Os_Lcfg.h"
#include "queue.h"
#include "sysmod/syslog.h"


typedef boolean bool_t;
#ifndef true
#define true TRUE
#endif
#ifndef false
#define false FALSE
#endif

#else
#include "t_stddef.h"
#include "cmsis_impl.h"
#include "kernel.h"
#ifndef TOPPERS_CB_TYPE_ONLY
#include "kernel_cfg.h"
#endif
#include "sil.h"

typedef int StatusType;
typedef int AlarmType;
typedef int TaskType;
typedef int* TaskRefType;
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
#endif /* OS_AUTOSAR */

#endif /* _AUTOSAR_OS_EXTEND_COMMON_H_ */
