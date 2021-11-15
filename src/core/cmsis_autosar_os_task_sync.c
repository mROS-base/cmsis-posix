#include "cmsis_autosar_os_task_sync.h"
#include "cmsis_autosar_os_time.h"
#include "autosar_os_ext_common_private.h"

typedef struct {
  CMSIS_IMPL_QUEUE	queue;
  TaskType			taskID;
  uint32_t			timeout;
  uint32_t			stick;
  uint32_t			expiretick;
  StatusType			ercd;
} AutosarOsTaskWaitInfoType;

typedef struct {
  CMSIS_IMPL_QUEUE			wait_queue;
  void						*data;
  AutosarOsTaskWaitInfoType		winfo;
} AutosarOsTaskWaitQueueEntryType;

static bool_t AutosarOsTaskIsTimeout(CMSIS_IMPL_QUEUE *entry, void *arg);
static void AutosarOsTaskWakeup(CMSIS_IMPL_QUEUE *entry, void *arg);
static bool_t AutosarOsTaskHasTargetId(CMSIS_IMPL_QUEUE *entry, void *arg);
static void AutosarOsTaskSyncWaitInfoInit(AutosarOsTaskWaitInfoType *winfop, uint32_t timeout, TaskType taskID);


static AutosarOsQueueHeadInitializer(autosar_os_task_sync_queue);

StatusType AutosarOsTaskSyncSleep(uint32_t timeout)
{
  AutosarOsTaskWaitInfoType winfo;
  TaskType taskID;

  StatusType ercd = GetTaskID(&taskID);
  if (ercd != E_OK) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d winfo.ercd=%d\n", __FILE__, __FUNCTION__, __LINE__, ercd);
    return winfo.ercd;
  }
  AutosarOsTaskSyncWaitInfoInit(&winfo, timeout, taskID);
  SuspendOSInterrupts();
  AutosarOsQueueHeadAddTail(&autosar_os_task_sync_queue, &winfo.queue);
  ResumeOSInterrupts();

  (void)WaitEvent(AutosarOsExtEvent);
  (void)ClearEvent(AutosarOsExtEvent);

  return E_OK;
}
void *AutosarOsTaskSyncWait(AutosarOsQueueHeadType *waiting_queue, uint32_t timeout, StatusType *ercdp, TaskType taskID)
{
  AutosarOsTaskWaitQueueEntryType wait_info;

  wait_info.data = NULL;
  AutosarOsTaskSyncWaitInfoInit(&wait_info.winfo, timeout, taskID);

  if (waiting_queue != NULL) {
    AutosarOsQueueHeadAddTail(waiting_queue, &wait_info.wait_queue);
  }
  AutosarOsQueueHeadAddTail(&autosar_os_task_sync_queue, &wait_info.winfo.queue);
  ResumeOSInterrupts();

  (void)WaitEvent(AutosarOsExtEvent);
  (void)ClearEvent(AutosarOsExtEvent);

  SuspendOSInterrupts();
  if (waiting_queue != NULL) {
    AutosarOsQueueHeadRemoveEntry(waiting_queue, &wait_info.wait_queue);
  }
  if (ercdp != NULL) {
    *ercdp = wait_info.winfo.ercd;
  }
  return wait_info.data;
}
bool_t AutosarOsTaskSyncWakeupFirstEntry(AutosarOsQueueHeadType *waiting_queue, void *data, StatusType ercd)
{
  AutosarOsTaskWaitQueueEntryType *wait_infop = (AutosarOsTaskWaitQueueEntryType*)waiting_queue->entries;
  if (wait_infop != NULL) {
    AutosarOsQueueHeadInitializer(dq);
    wait_infop->data = data;
    wait_infop->winfo.ercd = ercd;
    AutosarOsQueueHeadConditionalRemove(&autosar_os_task_sync_queue, &dq, AutosarOsTaskHasTargetId, &wait_infop->winfo.taskID);

    ResumeOSInterrupts();
    AutosarOsQueueHeadDoAction(&dq, AutosarOsTaskWakeup, &ercd);
    SuspendOSInterrupts();
    return true;
  } else {
    return false;
  }
}

StatusType AutosarOsTaskSyncWakeup(TaskType taskID, StatusType ercd)
{
  AutosarOsQueueHeadInitializer(dq);

  SuspendOSInterrupts();
  AutosarOsQueueHeadConditionalRemove(&autosar_os_task_sync_queue, &dq, AutosarOsTaskHasTargetId, &taskID);
  ResumeOSInterrupts();

  AutosarOsQueueHeadDoAction(&dq, AutosarOsTaskWakeup, &ercd);
  return E_OK;
}

static void TaskScheduleCallback(void)
{
  uint32_t curr;
  StatusType err = E_OS_LIMIT;
  AutosarOsQueueHeadInitializer(dq);

  AutosarOsTimeIncTickCount();
  if (autosar_os_task_sync_queue.entries == NULL) {
    return;
  }
  curr = AutosarOsTimeGetTickCount();

  SuspendOSInterrupts();
  AutosarOsQueueHeadConditionalRemove(&autosar_os_task_sync_queue, &dq, AutosarOsTaskIsTimeout, (void*)&curr);
  ResumeOSInterrupts();

  AutosarOsQueueHeadDoAction(&dq, AutosarOsTaskWakeup, &err);
  return;
}

TASK(AutosarOsExtCyclicTask)
{
  TaskScheduleCallback();
  TerminateTask();
  return;
}

static void AutosarOsTaskSyncWaitInfoInit(AutosarOsTaskWaitInfoType *winfop, uint32_t timeout, TaskType taskID)
{
  winfop->timeout = timeout;
  winfop->stick = AutosarOsTimeGetTickCount();
  winfop->expiretick = AutosarOsTimeGetExpireTickCount(winfop->stick, timeout);
  cmsis_impl_queue_initialize(&winfop->queue);
  winfop->taskID = taskID;
  winfop->ercd = E_OK;
  return;
}


static bool_t AutosarOsTaskIsTimeout(CMSIS_IMPL_QUEUE *entry, void *arg)
{
  uint32_t curr = *((uint32_t*)arg);
  AutosarOsTaskWaitInfoType *winfop = (AutosarOsTaskWaitInfoType *)entry;
  if (winfop->timeout == AUTOSAR_OS_TASK_SYNC_WAIT_FOREVER) {
    return false;
  }
  if (AutosarOsTimeIsTimeout(curr, winfop->stick, winfop->expiretick)) {
    return true;
  }
  return false;
}

static void AutosarOsTaskWakeup(CMSIS_IMPL_QUEUE *entry, void *arg)
{
  AutosarOsTaskWaitInfoType *winfop = (AutosarOsTaskWaitInfoType *)entry;
  StatusType ercd = *((StatusType*)arg);
  winfop->ercd = ercd;
  (void)SetEvent(winfop->taskID, AutosarOsExtEvent);
  return;
}

static bool_t AutosarOsTaskHasTargetId(CMSIS_IMPL_QUEUE *entry, void *arg)
{
  TaskType taskID = *((TaskType*)arg);
  AutosarOsTaskWaitInfoType *winfop = (AutosarOsTaskWaitInfoType *)entry;
  if (winfop->taskID == taskID) {
    return true;
  }
  return false;
}
