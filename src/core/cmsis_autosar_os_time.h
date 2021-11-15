#ifndef _CMSIS_AUTOSAR_OS_TIME_H_
#define _CMSIS_AUTOSAR_OS_TIME_H_

#include "autosar_os_ext_common.h"

extern uint32_t AutosarOsTimeGetTickCount(void);
extern void AutosarOsTimeIncTickCount(void);
extern uint32_t AutosarOsTimeGetExpireTickCount(uint32_t stick, uint32_t timeout);
extern bool_t AutosarOsTimeIsTimeout(uint32_t curr, uint32_t s, uint32_t e);


//TODO test func
extern void AutosarOsTimeIncTickCountSet(uint32_t set_time);

#endif /* _CMSIS_AUTOSAR_OS_TIME_H_ */
