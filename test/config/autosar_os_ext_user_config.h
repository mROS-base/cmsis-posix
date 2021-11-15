#ifndef _AUTOSAR_OS_EXT_CONFIG_H_
#define _AUTOSAR_OS_EXT_CONFIG_H_

#include "cmsis_os.h"
/***********************************************
 *
 * MEMORY
 *
 ***********************************************/
#define APP_HEAP_SIZE	(16 * 1024) /* 16KB */


/***********************************************
 *
 * THREAD
 *
 ***********************************************/
#define USER_THREAD_NUM		(7U)

extern void AutosarOsTaskTestConsumer1_Body(void *argument);
extern void AutosarOsTaskTestConsumer2_Body(void *argument);
extern void AutosarOsTaskTestConsumer3_Body(void *argument);

extern void AutosarOsTaskTestSupplier1_Body(void *argument);
extern void AutosarOsTaskTestSupplier2_Body(void *argument);
extern void AutosarOsTaskTestSupplier3_Body(void *argument);

#endif /* _AUTOSAR_OS_EXT_CONFIG_H_ */
