#ifndef _CMSIS_POSIX_OS_TIME_H_
#define _CMSIS_POSIX_OS_TIME_H_

#include "cmsis_os.h"

extern uint32_t PosixOsTimeGetTickCount(void);
extern osStatus_t PosixOsTimerInit(void);

#endif /* _CMSIS_POSIX_OS_TIME_H_ */
