#include "cmsis_os.h"
#include "autosar_os_ext_common.h"
#include "autosar_os_ext_sys_config.h"
#include "autosar_os_ext_common_private.h"

typedef struct {
    osThreadFunc_t func;
    void* argp;
} PosixOsThreadFuncType;

static void* posix_os_thread_func(void* argp)
{
    PosixOsThreadFuncType* fp = (PosixOsThreadFuncType*)argp;
    fp->func(fp->argp);
    return NULL;
}

osThreadId_t osThreadNew(osThreadFunc_t	func, void *argument, const osThreadAttr_t *attr)
{
    pthread_t thread_id;
    PosixOsThreadFuncType arg;

    if (CurrentContextIsISR()) {
        return NULL;
    }
    arg.func = func;
    arg.argp = argument;
    int err = pthread_create(&thread_id, NULL, posix_os_thread_func, &arg);
    if (err != 0) {
        //TODO ERROR
        return NULL;
    }

    return (osThreadId_t)thread_id;
}

osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
    if (CurrentContextIsISR()) {
        return osErrorISR;
    }
    pthread_exit(NULL);
    return E_OK;
}

/*
 * Version 1
 */
osThreadId osThreadCreate(const osThreadDef_t *thread_def, void *argument)
{
  if (thread_def == NULL) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d thread_def should not be NULL\n", __FILE__, __FUNCTION__, __LINE__);
    return NULL;
  } else if (thread_def->pthread == NULL) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d thread_def->pthread should not be NULL\n", __FILE__, __FUNCTION__, __LINE__);
  }
  return osThreadNew((osThreadFunc_t)thread_def->pthread, argument, NULL);
}
