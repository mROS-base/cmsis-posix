#include "cmsis_autosar_os_message_queue.h"
#include "cmsis_autosar_os_memory.h"
#include "cmsis_autosar_os_task_sync.h"
#include "autosar_os_ext_common_private.h"
#include <string.h>

PosixOsMessageQueueType *PosixOsMessageQueueCreate(PosixOsMessageQueueConfigType *config)
{
  uint32_t i;
  uint32_t entry_total_size;
  uint32_t msg_data_total_size;
  char *top;
  char *entry_top;
  char *msg_top;
  PosixOsMessageQueueType *qh;

  entry_total_size = sizeof(PosixOsMessageQueueEntryType) * config->prealloc_num;
  msg_data_total_size = config->entry_size * config->prealloc_num;

  top = PosixOsMemoryAlloc(sizeof(PosixOsMessageQueueType) + entry_total_size + msg_data_total_size);
  if (top == NULL) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d cannot allocate memory size=%d\n", __FILE__, __FUNCTION__, __LINE__, sizeof(PosixOsMessageQueueType) + entry_total_size + msg_data_total_size);
    return NULL;
  }
  qh = (PosixOsMessageQueueType *)top;
  qh->entry_size = config->entry_size;
  qh->prealloc_num = config->prealloc_num;
  qh->magicno = AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO;
  PosixOsQueueHeadInit(&qh->free);
  PosixOsQueueHeadInit(&qh->used);
  PosixOsQueueHeadInit(&qh->getter_waiting);
  PosixOsQueueHeadInit(&qh->putter_waiting);

  entry_top = &top[sizeof(PosixOsMessageQueueType)];
  msg_top = &entry_top[entry_total_size];
  /*
   *  [ queue memory image ]
   *
   *     qh     entry0 entry1   .................  entryN    msg0     msg1      ...................     msgN
   *   |------|------|------|------|------|------|------|----------|----------|----------|----------|----------|
   */
  PosixOsMessageQueueEntryType *entry = (PosixOsMessageQueueEntryType*)entry_top;
  for (i = 0; i < qh->prealloc_num; i++) {
    entry->prio = 0;
    entry->data = &msg_top[i * qh->entry_size];
    PosixOsQueueHeadAddTail(&qh->free,  &entry->queue);
    entry++;
  }
  return qh;
}

StatusType PosixOsMessageQueueDelete(PosixOsMessageQueueType *qh)
{
  if (qh == NULL) {
    return E_OS_ID;
  } else if (qh->magicno != AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO) {
    return E_OS_ID;
  }
  qh->magicno = 0;
  PosixOsMemoryFree(qh);
  return E_OK;
}

StatusType PosixOsMessageQueueGet(PosixOsMessageQueueType *qh, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
  StatusType ercd = E_OK;
  PosixOsMessageQueueEntryType *entry = NULL;
  TaskType taskID;

  if (qh->magicno != AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d invalid magicno=%d\n", __FILE__, __FUNCTION__, __LINE__, qh->magicno);
    return E_OS_ID;
  }
  if (!CurrentContextIsISR()) {
    ercd = GetTaskID(&taskID);
    if (ercd != E_OK) {
      CMSIS_IMPL_ERROR("ERROR:%s %s() %d GetTaskID() err=%d\n", __FILE__, __FUNCTION__, __LINE__, ercd);
      return ercd;
    }
  }

  SuspendOSInterrupts();
  entry = (PosixOsMessageQueueEntryType*)PosixOsQueueHeadRemoveFirst(&qh->used);
  if (entry == NULL) {
    if (timeout != 0) {
      entry = (PosixOsMessageQueueEntryType *)PosixOsTaskSyncWait(&qh->getter_waiting, timeout, NULL, taskID);
    }
  }
  if (entry != NULL) {
    memcpy(msg_ptr, entry->data, qh->entry_size);
    if (!PosixOsTaskSyncWakeupFirstEntry(&qh->putter_waiting, entry, E_OK)) {
      PosixOsQueueHeadAddTail(&qh->free, &entry->queue);
    } else {
      /* nothing to do */
    }
  } else {
    ercd = E_OS_LIMIT;
  }
  ResumeOSInterrupts();
  return ercd;
}
bool_t PosixOsMessageQueueIsValid(PosixOsMessageQueueType *qh)
{
  return (qh->magicno == AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO);
}

StatusType PosixOsMessageQueuePut(PosixOsMessageQueueType *qh, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
  StatusType ercd = E_OK;
  PosixOsMessageQueueEntryType *entry = NULL;
  TaskType taskID;

  if (qh->magicno != AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO) {
    CMSIS_IMPL_ERROR("ERROR:%s %s() %d invalid magicno=%d\n", __FILE__, __FUNCTION__, __LINE__, qh->magicno);
    return E_OS_ID;
  }
  if (!CurrentContextIsISR()) {
    ercd = GetTaskID(&taskID);
    if (ercd != E_OK) {
      CMSIS_IMPL_ERROR("ERROR:%s %s() %d GetTaskID() err=%d\n", __FILE__, __FUNCTION__, __LINE__, ercd);
      return ercd;
    }
  }

  SuspendOSInterrupts();
  entry = (PosixOsMessageQueueEntryType*)PosixOsQueueHeadRemoveFirst(&qh->free);
  if (entry == NULL) {
    if (timeout != 0) {
      entry = (PosixOsMessageQueueEntryType*)PosixOsTaskSyncWait(&qh->putter_waiting, timeout, NULL, taskID);
    }
  }
  if (entry != NULL) {
    memcpy(entry->data, msg_ptr, qh->entry_size);
    if (!PosixOsTaskSyncWakeupFirstEntry(&qh->getter_waiting, entry, E_OK)) {
      (void)PosixOsQueueHeadAddTail(&qh->used, &entry->queue);
    } else {
      /* nothing to do */
    }
  } else {
    ercd = E_OS_LIMIT;
  }
  ResumeOSInterrupts();
  return ercd;
}
