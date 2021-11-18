#include "cmsis_os.h"
#include "cmsis_posix_os_memory.h"

typedef struct {
    osThreadFunc_t func;
    void* argument;
} PosixOsThreadFuncType;

static void* posix_os_thread_func(void* argp)
{
    osThreadFunc_t func = ((PosixOsThreadFuncType*)argp)->func;
    void* argument = ((PosixOsThreadFuncType*)argp)->argument;

    PosixOsMemoryFree(argp);
    func(argument);
    return NULL;
}

osThreadId_t osThreadNew(osThreadFunc_t	func, void* argument, const osThreadAttr_t* attr)
{
    osThreadId_t ret = (osThreadId_t)NULL;
    pthread_t thread_id;
    PosixOsThreadFuncType *argp;

    if (CurrentContextIsISR()) {
        return NULL;
    }
    argp = (PosixOsThreadFuncType*)PosixOsMemoryAlloc(sizeof(PosixOsThreadFuncType));
    if (argp == NULL) {
        return NULL;
    }
    argp->func = func;
    argp->argument = argument;
    int err = pthread_create(&thread_id, NULL, posix_os_thread_func, argp);
    if (err == 0) {
        ret = (osThreadId_t)thread_id;
    }
    else {
        //TODO ERROR
    }

    return (osThreadId_t)ret;
}

osStatus_t osThreadTerminate(osThreadId_t thread_id)
{
    if (CurrentContextIsISR()) {
        return osErrorISR;
    }
    pthread_exit(NULL);
    return osOK;
}

/*
 * Version 1
 */
osThreadId osThreadCreate(const osThreadDef_t* thread_def, void* argument)
{
    if (thread_def == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d thread_def should not be NULL\n", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    else if (thread_def->pthread == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d thread_def->pthread should not be NULL\n", __FILE__, __FUNCTION__, __LINE__);
    }
    return osThreadNew((osThreadFunc_t)thread_def->pthread, argument, NULL);
}
