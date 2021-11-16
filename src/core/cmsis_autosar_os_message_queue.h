#ifndef _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_
#define _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_

#include "cmsis_autosar_os_queue.h"
#include "cmsis_autosar_os_types.h"

#define AUTOSAR_OSMESSAGE_QUEUE_HEAD_MAGICNO		0xDEAFBEAD
typedef struct {
  uint16_t			prealloc_num;
  uint16_t			entry_size;
  uint32_t			magicno;
  PosixOsQueueHeadType	used;
  PosixOsQueueHeadType	free;
  PosixOsQueueHeadType	getter_waiting;
  PosixOsQueueHeadType	putter_waiting;
} PosixOsMessageQueueType;


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
} PosixOsMessageQueueEntryType;


typedef struct {
  uint16_t 					prealloc_num;
  uint16_t 					entry_size;
  PosixOsMessageQueueEntryType   *control_datap;
  void 	 					*entries_datap;
} PosixOsMessageQueueConfigType;
extern PosixOsMessageQueueType *PosixOsMessageQueueCreate(PosixOsMessageQueueConfigType *config);
extern StatusType PosixOsMessageQueueDelete(PosixOsMessageQueueType *qh);

extern StatusType PosixOsMessageQueueGet(PosixOsMessageQueueType *qh, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout);
extern StatusType PosixOsMessageQueuePut(PosixOsMessageQueueType *qh, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout);
extern bool_t PosixOsMessageQueueIsValid(PosixOsMessageQueueType *qh);

#endif /* _CMSIS_AUTOSAR_OS_MESSAGE_QUEUE_H_ */
