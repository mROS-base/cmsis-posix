#ifndef _CMSIS_AUTOSAR_OS_MEMORY_H_
#define _CMSIS_AUTOSAR_OS_MEMORY_H_

#include "autosar_os_ext_common.h"

extern void *AutosarOsMemoryAlloc(uint32_t size);
extern void AutosarOsMemoryFree(void *addrp);

#endif /* _CMSIS_AUTOSAR_OS_MEMORY_H_ */
