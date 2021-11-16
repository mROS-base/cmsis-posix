#include "cmsis_autosar_os_task_sync.h"
#include "autosar_os_ext_common_private.h"
#include "cmsis_os.h"

osStatus_t osDelay(uint32_t ticks)
{
    if (CurrentContextIsISR()) {
        return osErrorISR;
    }
    if (ticks == 0) {
        return osErrorParameter;
    }
    return PosixOsThreadSyncWait(ticks);
}
