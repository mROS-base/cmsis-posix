#include "cmsis_autosar_os_time.h"
#include "cmsis_os.h"

uint32_t osKernelGetTickCount(void)
{
  return AutosarOsTimeGetTickCount();
}
