#ifndef _AUTOSAR_OS_EXTEND_COMMON_PRIVATE_H_
#define _AUTOSAR_OS_EXTEND_COMMON_PRIVATE_H_

#include <pthread.h>
#include <time.h>
#include <errno.h>

#include "cmsis_impl.h"

extern bool_t CurrentContextIsISR(void);

#define TIMESPEC_NANOSEC    1000000000LLU
#define TIMESPEC_MSEC       1000LLU


#endif /* _AUTOSAR_OS_EXTEND_COMMON_H_ */
