#ifndef _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_
#define _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_

#include "cmsis_autosar_os_queue.h"
#include "cmsis_autosar_os_types.h"

#define AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO		0xDEAFBEAD
typedef struct {
  uint16_t			prealloc_num;
  uint16_t			entry_size;
  uint32_t			magicno;
  AutosarOsQueueHeadType	used;
  AutosarOsQueueHeadType	free;
  AutosarOsQueueHeadType	getter_waiting;
  AutosarOsQueueHeadType	putter_waiting;
} AutosarOsMessageQueueType;


typedef struct {
  CMSIS_IMPL_QUEUE						queue;
  /*
   * Priority
   *
   *  Highest  UINT16_MAX
   *  Lowest   0
   */
  uint16_t					prio;
  void						*data;
} AutosarOsMessageQueueEntryType;


typedef struct {
  uint16_t 					prealloc_num;
  uint16_t 					entry_size;
  AutosarOsMessageQueueEntryType   *control_datap;
  void 	 					*entries_datap;
} AutosarOsMessageQueueConfigType;
extern AutosarOsMessageQueueType *AutosarOsMessageQueueCreate(AutosarOsMessageQueueConfigType *config);
extern StatusType AutosarOsMessageQueueDelete(AutosarOsMessageQueueType *qh);

extern StatusType AutosarOsMessageQueueGet(AutosarOsMessageQueueType *qh, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
extern StatusType AutosarOsMessageQueuePut(AutosarOsMessageQueueType *qh, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);
extern bool_t AutosarOsMessageQueueIsValid(AutosarOsMessageQueueType *qh);

#endif /* _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_ */
