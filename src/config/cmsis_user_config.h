#ifndef _CMSIS_USER_CONFIG_H_
#define _CMSIS_USER_CONFIG_H_

#include "cmsis_config_utl.h"

extern UserThreadAutosarOsTaskMapEntryType* cmsis_get_user_thread_map(void);
extern int cmsis_get_app_heap_size(void);
extern int cmsis_get_user_thread_num(void);

extern const AutosarOsExtFlagMapEntryType* cmsis_get_flag_user_map(void);
extern int cmsis_get_flag_user_num(void);

#endif /* _CMSIS_USER_CONFIG_H_ */
