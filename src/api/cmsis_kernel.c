#include "cmsis_posix_os_time.h"
#include "cmsis_posix_os_task_sync.h"
#include "cmsis_os.h"

osStatus_t osKernelStart(void)
{
	PosixOsThreadSyncInit();
	(void)PosixOsTimerInit();
	CMSIS_IMPL_INFO("osKernelStart");
	return osOK;
}

uint32_t osKernelGetTickCount(void)
{
	return PosixOsTimeGetTickCount();
}
