#ifndef _CMSIS_AUTOSAR_OS_QUEUE_H_
#define _CMSIS_AUTOSAR_OS_QUEUE_H_

#include "autosar_os_ext_common.h"

typedef struct {
  uint16_t	count;
  CMSIS_IMPL_QUEUE		*entries;
} AutosarOsQueueHeadType;

#define AutosarOsQueueHeadInitializer(var)	\
		AutosarOsQueueHeadType var = { .count = 0, .entries = NULL }
#define AutosarOsQueueHeadInit(qheadp)	\
do {	\
	(qheadp)->count = 0;	\
	(qheadp)->entries = NULL;	\
} while (0)

extern void AutosarOsQueueHeadAddTail(AutosarOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry);
extern CMSIS_IMPL_QUEUE* AutosarOsQueueHeadRemoveFirst(AutosarOsQueueHeadType *headp);
extern void AutosarOsQueueHeadRemoveEntry(AutosarOsQueueHeadType *headp, CMSIS_IMPL_QUEUE *entry);
extern void AutosarOsQueueHeadConditionalRemove(AutosarOsQueueHeadType *srcq, AutosarOsQueueHeadType *dstq, bool_t (*cond_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg);
extern void AutosarOsQueueHeadDoAction(AutosarOsQueueHeadType *headp, void (*act_func) (CMSIS_IMPL_QUEUE *entry, void *arg), void *arg);

#endif /* _CMSIS_AUTOSAR_OS_QUEUE_H_ */
