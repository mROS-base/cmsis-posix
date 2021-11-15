#include "cmsis_autosar_os_memory.h"
#include "autosar_os_ext_common_private.h"
#include <stdlib.h>

void *AutosarOsMemoryAlloc(uint32_t size)
{
  void *addrp = NULL;

  if (CurrentContextIsISR()) {
    return NULL;
  }
  SuspendOSInterrupts();
  addrp = malloc(size);
  ResumeOSInterrupts();

  return addrp;
}

void AutosarOsMemoryFree(void *addrp)
{
  if (CurrentContextIsISR()) {
    return;
  }
  if (addrp != NULL) {
    SuspendOSInterrupts();
    free(addrp);
    ResumeOSInterrupts();
  }
  return;
}
