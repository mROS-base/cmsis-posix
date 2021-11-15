#ifndef _CMSIS_TYPES_H_
#define _CMSIS_TYPES_H_

#ifdef OS_POSIX
#include <stdint.h>
#include <assert.h>
typedef unsigned short uint16_t;
typedef unsigned int bool_t;
#define Inline static inline
#define false 0U
#define true (!false)
#else
#include <tool_stddef.h>
#endif


#endif /* _CMSIS_TYPES_H_ */
