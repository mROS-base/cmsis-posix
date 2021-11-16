#include "cmsis_posix_os_thread_sync.h"
#include "cmsis_posix_os_time.h"
#include "posix_os_ext_common_private.h"

typedef struct {
    CMSIS_IMPL_QUEUE	queue;
    uint32_t			timeout;
    uint32_t			stick;
    osStatus_t			ercd;
    pthread_cond_t      cond;
} PosixOsTaskWaitInfoType;

typedef struct {
    CMSIS_IMPL_QUEUE			wait_queue;
    void* data;
    PosixOsTaskWaitInfoType		winfo;
} PosixOsTaskWaitQueueEntryType;

static void PosixOsTaskSyncWaitInfoInit(PosixOsTaskWaitInfoType* winfop, uint32_t timeout);


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
    uint64_t timeout64 = timeout;
    clock_gettime(CLOCK_REALTIME, tmop);
    //printf("tv_sec=%ld\n", tmop->tv_sec);
    //printf("tv_nsec=%ld\n", tmop->tv_nsec);
    tmop->tv_nsec += (timeout64 * TIMESPEC_MSEC * TIMESPEC_MSEC);
    //printf("add:tv_nsec=%ld\n", tmop->tv_nsec);
    if (tmop->tv_nsec >= TIMESPEC_NANOSEC) {
        struct timespec over_tmo;
        over_tmo.tv_sec = (tmop->tv_nsec / TIMESPEC_NANOSEC);
        over_tmo.tv_nsec = (over_tmo.tv_sec * TIMESPEC_NANOSEC);
        //printf("over_tmo:tv_sec=%ld\n", over_tmo.tv_sec);
        //printf("over_tmo:tv_nsec=%ld\n", over_tmo.tv_nsec);
        tmop->tv_sec += over_tmo.tv_sec;
        tmop->tv_nsec -= over_tmo.tv_nsec;
    }
    //printf("tv_sec=%ld\n", tmop->tv_sec);
    //printf("tv_nsec=%ld\n", tmop->tv_nsec);
    return;
}

osStatus_t PosixOsThreadSyncSleep(uint32_t timeout)
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

void* PosixOsThreadSyncWait(PosixOsQueueHeadType* waiting_queue, uint32_t timeout, osStatus_t* ercdp)
{
    PosixOsTaskWaitQueueEntryType wait_info;
    struct timespec tmo;

    wait_info.data = NULL;
    PosixOsTaskSyncWaitInfoInit(&wait_info.winfo, timeout);

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
            *ercdp = osError; //TODO
        }
        else {
            *ercdp = wait_info.winfo.ercd;
        }
    }
    return wait_info.data;
}
bool_t PosixOsThreadSyncWakeupFirstEntry(PosixOsQueueHeadType* waiting_queue, void* data, osStatus_t ercd)
{
    PosixOsTaskWaitQueueEntryType *wait_infop = (PosixOsTaskWaitQueueEntryType*)PosixOsQueueHeadRemoveFirst(waiting_queue);
    if (wait_infop != NULL) {
        wait_infop->data = data;
        wait_infop->winfo.ercd = ercd;
        pthread_cond_signal(&wait_infop->winfo.cond);
        return true;
    }
    else {
        return false;
    }
}


static void PosixOsTaskSyncWaitInfoInit(PosixOsTaskWaitInfoType* winfop, uint32_t timeout)
{
    winfop->timeout = timeout;
    winfop->stick = PosixOsTimeGetTickCount();
    cmsis_impl_queue_initialize(&winfop->queue);
    winfop->ercd = osOK;
    pthread_cond_init(&winfop->cond, NULL);
    return;
}

