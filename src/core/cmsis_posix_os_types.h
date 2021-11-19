#ifndef _CMSIS_POSIX_OS_TYPES_H_
#define _CMSIS_POSIX_OS_TYPES_H_

#define POSIX_OS_THREAD_SYNC_WAIT_FOREVER		(-1U)

extern bool_t CurrentContextIsISR(void);

#define TIMESPEC_NANOSEC    1000000000LLU
#define TIMESPEC_MSEC       1000LLU

#define _GNU_SOURCE
#include <pthread.h>
#include <time.h>
#include <errno.h>


#endif /* _CMSIS_POSIX_OS_TYPES_H_ */
