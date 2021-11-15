#include "cmsis_os.h"
#include "cmsis_user_config.h"
#include "autosar_os_ext_user_config.h"

UserThreadAutosarOsTaskMapEntryType user_thread_autosar_ostask_map[USER_THREAD_NUM] = {
  {
    .taskID = AutosarOsTaskTestControl,
    .func = NULL,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestSupplier1,
    .func = AutosarOsTaskTestSupplier1_Body,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestSupplier2,
    .func = AutosarOsTaskTestSupplier2_Body,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestSupplier3,
    .func = AutosarOsTaskTestSupplier3_Body,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestConsumer1,
    .func = AutosarOsTaskTestConsumer1_Body,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestConsumer2,
    .func = AutosarOsTaskTestConsumer2_Body,
    .argument = NULL,
  },
  {
    .taskID = AutosarOsTaskTestConsumer3,
    .func = AutosarOsTaskTestConsumer3_Body,
    .argument = NULL,
  },
};

UserThreadAutosarOsTaskMapEntryType* cmsis_get_user_thread_map(void)
{
  return user_thread_autosar_ostask_map;
}
int cmsis_get_app_heap_size(void)
{
  return APP_HEAP_SIZE;
}
int cmsis_get_user_thread_num(void)
{
  return USER_THREAD_NUM;
}
