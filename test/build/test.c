#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test.h"
#include "stm32f7xx_hal.h"

#include "cmsis_os.h"

UART_HandleTypeDef huart3;

/*
 *  サービスコールのエラーのログ出力
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
  if (ercd < 0) {
    t_perror(LOG_ERROR, file, line, expr, ercd);
  }
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))


/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
  osStatus_t ret;

  SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
  syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

  ret = osThreadTerminate(NULL);
  if (ret != osOK) {
    syslog(LOG_NOTICE, "main_task() task terminate error %d", ret);
  }
}
