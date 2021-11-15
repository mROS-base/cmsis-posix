#include "autosar_os_ext_asp3_user_config.h"
#include "autosar_os_ext_common_private.h"

const AutosarOsExtFlagMapEntryType autosar_os_ext_flag_user_map[AUTOSAR_OS_EXT_ASP3_FLAG_USER_NUM] = {
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestControl),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestSupplier1),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestSupplier2),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestSupplier3),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestConsumer1),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestConsumer2),
  AUTOSAR_OS_EXT_FLAG_MAP_ENTRY(AutosarOsTaskTestConsumer3),


};

const AutosarOsExtFlagMapEntryType* cmsis_get_flag_user_map(void)
{
  return autosar_os_ext_flag_user_map;
}

int cmsis_get_flag_user_num(void)
{
  return AUTOSAR_OS_EXT_ASP3_FLAG_USER_NUM;
}

