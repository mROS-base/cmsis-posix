#ifndef _CMSIS_AUTOSAR_OS_TIME_H_
#define _CMSIS_AUTOSAR_OS_TIME_H_

#include "posix_os_ext_common_private.h"
#include "cmsis_os.h"

extern uint32_t PosixOsTimeGetTickCount(void);
extern osStatus_t PosixOsTimerInit(void);

#endif /* _CMSIS_AUTOSAR_OS_TIME_H_ */
