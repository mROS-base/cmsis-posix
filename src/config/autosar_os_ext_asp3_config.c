#include "autosar_os_ext_asp3_config.h"
#include "cmsis_user_config.h"
#include "autosar_os_ext_asp3_sys_config.h"

ER AutosarOsExtTask2EventFlag(ID taskID, ID *flgid)
{
  uint32_t i;

  int flag_user_num;
  const AutosarOsExtFlagMapEntryType *user_map_p;

  user_map_p = cmsis_get_flag_user_map();
  flag_user_num = cmsis_get_flag_user_num();

  for (i = 0; i < flag_user_num; i++) {
    if (user_map_p[i].taskID == taskID) {
      *flgid = user_map_p[i].flgid;
      return E_OK;
    }
  }
  for (i = 0; i < AUTOSAR_OS_EXT_ASP3_FLAG_SYS_NUM; i++) {
    if (autosar_os_ext_flag_sys_map[i].taskID == taskID) {
      *flgid = autosar_os_ext_flag_sys_map[i].flgid;
      return E_OK;
    }
  }
  return E_OS_ID;
}
