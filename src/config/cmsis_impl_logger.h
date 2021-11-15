#ifndef _CMSIS_IMPL_LOGGER_H_
#define _CMSIS_IMPL_LOGGER_H_

#ifdef CMSIS_LOG_DISABLE_DEBUG
#else
#include "t_syslog.h"
#endif

/*
 * CMSIS LOG API
 */
#ifdef CMSIS_LOG_DISABLE_DEBUG
#define CMSIS_IMPL_DEBUG(...)
#else
#define CMSIS_IMPL_DEBUG(...) 		syslog(LOG_DEBUG, __VA_ARGS__)
#endif

#ifdef CMSIS_LOG_DISABLE_INFO
#define CMSIS_IMPL_INFO(...)
#else
#define CMSIS_IMPL_INFO(...)       syslog(LOG_NOTICE, __VA_ARGS__)
#endif

#ifdef CMSIS_LOG_DISABLE_WARN
#define CMSIS_IMPL_WARN(...)
#else
#define CMSIS_IMPL_WARN(...)       syslog(LOG_WARNING, __VA_ARGS__)
#endif

#ifdef CMSIS_LOG_DISABLE_ERROR
#define CMSIS_IMPL_ERROR(...)
#else
#define CMSIS_IMPL_ERROR(...)      syslog(LOG_ERROR, __VA_ARGS__)
#endif

#ifdef CMSIS_LOG_DISABLE_FATAL
#define CMSIS_IMPL_FATAL(...)
#else
#define CMSIS_IMPL_FATAL(...)      syslog(LOG_EMERG, __VA_ARGS__)
#endif



#endif /* _CMSIS_IMPL_LOGGER_H_ */
