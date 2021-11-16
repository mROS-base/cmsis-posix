#ifndef _CMSIS_AUTOSAR_OS_MEMORY_H_
#define _CMSIS_AUTOSAR_OS_MEMORY_H_

#include "posix_os_ext_common_private.h"

extern void *PosixOsMemoryAlloc(uint32_t size);
extern void PosixOsMemoryFree(void *addrp);

#endif /* _CMSIS_AUTOSAR_OS_MEMORY_H_ */
