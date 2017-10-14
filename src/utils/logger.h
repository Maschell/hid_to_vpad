#ifndef __LOGGER_H_
#define __LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#define __FILENAME_X__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILENAME_X__)

#define DEBUG_FUNCTION_LINE(FMT, ARGS...)do { \
    log_printf("[%23s]%30s@L%04d: " FMT "",__FILENAME__,__FUNCTION__, __LINE__, ## ARGS); \
    } while (0)


#define DEBUG_LOGGER        0

#ifdef DEBUG_LOGGER
void log_init();
void log_deinit(void);
void log_print(const char *str);
void log_printf(const char *format, ...);
#else
#define log_init()
#define log_print(x)
#define log_printf(x, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif
