#include "cmsis_autosar_os_task_sync.h"
#include "cmsis_autosar_os_time.h"
#include "autosar_os_ext_common_private.h"

typedef struct {
    CMSIS_IMPL_QUEUE	queue;
    TaskType			taskID;
    uint32_t			timeout;
    uint32_t			stick;
    StatusType			ercd;
    pthread_cond_t      cond;
} PosixOsTaskWaitInfoType;

typedef struct {
    CMSIS_IMPL_QUEUE			wait_queue;
    void* data;
    PosixOsTaskWaitInfoType		winfo;
} PosixOsTaskWaitQueueEntryType;

static void PosixOsTaskWakeup(CMSIS_IMPL_QUEUE* entry, void* arg);
static bool_t PosixOsTaskHasTargetId(CMSIS_IMPL_QUEUE* entry, void* arg);
static void PosixOsTaskSyncWaitInfoInit(PosixOsTaskWaitInfoType* winfop, uint32_t timeout, TaskType taskID);


static PosixOsQueueHeadInitializer(autosar_os_task_sync_queue);

static pthread_mutex_t posix_os_mutex;
static pthread_cond_t posix_os_cond;

void PosixOsThreadSyncInit(void)
{
    pthread_mutex_init(&posix_os_mutex, NULL);
    pthread_cond_init(&posix_os_cond, NULL);
    return;
}

void PosixOsThreadSyncLock(void)
{
    pthread_mutex_lock(&posix_os_mutex);
    return;
}
void PosixOsThreadSyncUnlock(void)
{
    pthread_mutex_unlock(&posix_os_mutex);
    return;
}

static void add_timespec(struct timespec* tmop, uint32_t timeout)
{
    clock_gettime(CLOCK_MONOTONIC, tmop);
    tmop->tv_sec += (timeout / TIMESPEC_MSEC);
    tmop->tv_nsec += ((timeout % TIMESPEC_MSEC) * TIMESPEC_MSEC);
    if (tmop->tv_nsec >= TIMESPEC_NANOSEC) {
        struct timespec over_tmo;
        over_tmo.tv_sec = (tmop->tv_nsec / TIMESPEC_NANOSEC);
        over_tmo.tv_nsec = (over_tmo.tv_sec * TIMESPEC_NANOSEC);
        tmop->tv_sec += over_tmo.tv_sec;
        tmop->tv_nsec -= over_tmo.tv_nsec;
    }
    return;
}

osStatus_t PosixOsThreadSyncWait(uint32_t timeout)
{
    osStatus_t ret = osOK;
    struct timespec tmo;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_mutex_lock(&mutex);
    add_timespec(&tmo, timeout);
    int err = pthread_cond_timedwait(&cond, &mutex, &tmo);
    if (err != ETIMEDOUT) {
        ret = osError;
    }
    pthread_mutex_unlock(&mutex);
    return ret;
}

void* PosixOsTaskSyncWait(PosixOsQueueHeadType* waiting_queue, uint32_t timeout, StatusType* ercdp, TaskType taskID)
{
    PosixOsTaskWaitQueueEntryType wait_info;
    struct timespec tmo;

    wait_info.data = NULL;
    PosixOsTaskSyncWaitInfoInit(&wait_info.winfo, timeout, taskID);

    if (waiting_queue != NULL) {
        PosixOsQueueHeadAddTail(waiting_queue, &wait_info.wait_queue);
    }
    PosixOsQueueHeadAddTail(&autosar_os_task_sync_queue, &wait_info.winfo.queue);

    add_timespec(&tmo, timeout);
    int err = pthread_cond_timedwait(&wait_info.winfo.cond, &posix_os_mutex, &tmo);
    if (waiting_queue != NULL) {
        PosixOsQueueHeadRemoveEntry(waiting_queue, &wait_info.wait_queue);
    }
    if (ercdp != NULL) {
        if ((err != 0) && (err != ETIMEDOUT)) {
            *ercdp = E_OS_ID; //TODO
        }
        else {
            *ercdp = wait_info.winfo.ercd;
        }
    }
    return wait_info.data;
}
bool_t PosixOsTaskSyncWakeupFirstEntry(PosixOsQueueHeadType* waiting_queue, void* data, StatusType ercd)
{
    PosixOsTaskWaitQueueEntryType* wait_infop = (PosixOsTaskWaitQueueEntryType*)waiting_queue->entries;
    if (wait_infop != NULL) {
        PosixOsQueueHeadInitializer(dq);
        wait_infop->data = data;
        wait_infop->winfo.ercd = ercd;
        PosixOsQueueHeadConditionalRemove(&autosar_os_task_sync_queue, &dq, PosixOsTaskHasTargetId, &wait_infop->winfo.taskID);
        PosixOsQueueHeadDoAction(&dq, PosixOsTaskWakeup, &ercd);
        return true;
    }
    else {
        return false;
    }
}


static void PosixOsTaskSyncWaitInfoInit(PosixOsTaskWaitInfoType* winfop, uint32_t timeout, TaskType taskID)
{
    winfop->timeout = timeout;
    winfop->stick = PosixOsTimeGetTickCount();
    cmsis_impl_queue_initialize(&winfop->queue);
    winfop->taskID = taskID;
    winfop->ercd = E_OK;
    pthread_cond_init(&winfop->cond, NULL);
    return;
}



static void PosixOsTaskWakeup(CMSIS_IMPL_QUEUE* entry, void* arg)
{
    PosixOsTaskWaitInfoType* winfop = (PosixOsTaskWaitInfoType*)entry;
    StatusType ercd = *((StatusType*)arg);
    winfop->ercd = ercd;
    pthread_cond_signal(&winfop->cond);
    return;
}

static bool_t PosixOsTaskHasTargetId(CMSIS_IMPL_QUEUE* entry, void* arg)
{
    TaskType taskID = *((TaskType*)arg);
    PosixOsTaskWaitInfoType* winfop = (PosixOsTaskWaitInfoType*)entry;
    if (winfop->taskID == taskID) {
        return true;
    }
    return false;
}
