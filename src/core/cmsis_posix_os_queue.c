#include "cmsis_posix_os_queue.h"
#include "cmsis_posix_os_memory.h"

void PosixOsQueueHeadAddTail(PosixOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry)
{
  cmsis_impl_queue_initialize(entry);
  if (headp->entries == NULL) {
    headp->entries = entry;
  } else {
    //add last
    cmsis_impl_queue_insert_prev(headp->entries, entry);
  }
  headp->count++;
  return;
}
void PosixOsQueueHeadRemoveEntry(PosixOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry)
{
  if (headp->entries != NULL) {
    CMSIS_IMPL_QUEUE *next = entry->p_next;
    cmsis_impl_queue_delete(entry);
    headp->count--;
    if (headp->count == 0) {
      headp->entries = NULL;
    } else if (headp->entries == entry) {
      headp->entries = next;
    }
  }
  return;
}
CMSIS_IMPL_QUEUE* PosixOsQueueHeadRemoveFirst(PosixOsQueueHeadType *headp)
{
  CMSIS_IMPL_QUEUE *first = headp->entries;
  if (first != NULL) {
    PosixOsQueueHeadRemoveEntry(headp, first);
  }
  return first;
}
void PosixOsQueueHeadConditionalRemove(PosixOsQueueHeadType *srcq, PosixOsQueueHeadType *dstq, bool_t (*cond_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg)
{
  uint32_t i;
  uint32_t count = srcq->count;
  CMSIS_IMPL_QUEUE *entry = srcq->entries;
  CMSIS_IMPL_QUEUE *next;

  for (i = 0; i < count; i++) {
    next = entry->p_next;
    if (cond_func(entry, arg)) {
      PosixOsQueueHeadRemoveEntry(srcq, entry);
      PosixOsQueueHeadAddTail(dstq, entry);
    }
    entry = next;
  }
  return;
}
void PosixOsQueueHeadDoAction(PosixOsQueueHeadType *headp, void (*act_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg)
{
  uint32_t i;
  uint32_t count = headp->count;
  CMSIS_IMPL_QUEUE *entry = headp->entries;
  CMSIS_IMPL_QUEUE *next;
  for (i = 0; i < count; i++) {
    next = entry->p_next;
    act_func(entry, arg);
    entry = next;
  }
}

