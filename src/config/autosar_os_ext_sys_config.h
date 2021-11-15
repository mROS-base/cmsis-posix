#ifndef _AUTOSAR_OS_EXT_CONFIG_UTILS_H_
#define _AUTOSAR_OS_EXT_CONFIG_UTILS_H_

#include "autosar_os_ext_common.h"
#include "cmsis_os.h"

/***********************************************
 *
 * THREAD
 *
 ***********************************************/

typedef struct {
  TaskType			taskID;
  osThreadFunc_t		func;
  void 				*argument;
} UserThreadAutosarOsTaskMapEntryType;

extern StatusType AutosarOsTaskConfigSet(const osThreadFunc_t func, void *argument);
extern StatusType AutosarOsTaskConfigGet(UserThreadAutosarOsTaskMapEntryType *config);
extern StatusType AutosarOsTaskConfigGetTaskID(const osThreadFunc_t func, TaskType *taskID);

/***********************************************
 *
 * MESSAGE QUEUE
 *
 ***********************************************/
#define MESSAGE_QUEUE_ENTRY_ALIGNMENT_SIZE	4U
#define MESSAGE_QUEUE_ENTRY_ALIGNED_SIZE(size)	\
	(	\
			(	\
				(	\
						(size) + (MESSAGE_QUEUE_ENTRY_ALIGNMENT_SIZE - 1)	\
				)	\
				/ MESSAGE_QUEUE_ENTRY_ALIGNMENT_SIZE	\
			)	\
			* MESSAGE_QUEUE_ENTRY_ALIGNMENT_SIZE	\
	)

#endif /* _AUTOSAR_OS_EXT_CONFIG_UTILS_H_ */
