#ifndef _CMSIS_IMPLE_H_
#define _CMSIS_IMPLE_H_

#include "cmsis_impl_logger.h"
#include "cmsis_impl_queue.h"
#include "cmsis_impl_types.h"

#include <pthread.h>
#include <time.h>
#include <errno.h>

extern bool_t CurrentContextIsISR(void);

#define TIMESPEC_NANOSEC    1000000000LLU
#define TIMESPEC_MSEC       1000LLU

#endif /* _CMSIS_IMPLE_H_ */

