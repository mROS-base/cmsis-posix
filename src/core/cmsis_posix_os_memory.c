#include "cmsis_posix_os_memory.h"
#include "cmsis_posix_os_types.h"
#include <stdlib.h>

void* PosixOsMemoryAlloc(uint32_t size)
{
    void* addrp = NULL;

    if (CurrentContextIsISR()) {
        return NULL;
    }
    addrp = malloc(size);
    return addrp;
}

void PosixOsMemoryFree(void* addrp)
{
    if (CurrentContextIsISR()) {
        return;
    }
    if (addrp != NULL) {
        free(addrp);
    }
    return;
}
