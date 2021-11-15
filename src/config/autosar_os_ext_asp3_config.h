#ifndef _AUTOSAR_OS_EXT_ASP3_CONFIG_H_
#define _AUTOSAR_OS_EXT_ASP3_CONFIG_H_

#include "autosar_os_ext_common.h"

typedef struct {
  ID taskID;
  ID flgid;
} AutosarOsExtFlagMapEntryType;

#define AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(taskname)	\
	{ .taskID = taskname, .flgid = taskname ## _FLG }

extern ER AutosarOsExtTask2EventFlag(ID taskID, ID *flgid);

#endif /* _AUTOSAR_OS_EXT_ASP3_CONFIG_H_ */
