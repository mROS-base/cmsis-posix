#ifndef _CMSIS_AUTOSAR_OS_QUEUE_H_
#define _CMSIS_AUTOSAR_OS_QUEUE_H_

#include "autosar_os_ext_common.h"

typedef struct {
  uint16_t	count;
  CMSIS_IMPL_QUEUE		*entries;
} PosixOsQueueHeadType;

#define PosixOsQueueHeadInitializer(var)	\
		PosixOsQueueHeadType var = { .count = 0, .entries = NULL }
#define PosixOsQueueHeadInit(qheadp)	\
do {	\
	(qheadp)->count = 0;	\
	(qheadp)->entries = NULL;	\
} while (0)

extern void PosixOsQueueHeadAddTail(PosixOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry);
extern CMSIS_IMPL_QUEUE* PosixOsQueueHeadRemoveFirst(PosixOsQueueHeadType *headp);
extern void PosixOsQueueHeadRemoveEntry(PosixOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry);
extern void PosixOsQueueHeadConditionalRemove(PosixOsQueueHeadType *srcq, PosixOsQueueHeadType *dstq, bool_t (*cond_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg);
extern void PosixOsQueueHeadDoAction(PosixOsQueueHeadType *headp, void (*act_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg);

#endif /* _CMSIS_AUTOSAR_OS_QUEUE_H_ */
