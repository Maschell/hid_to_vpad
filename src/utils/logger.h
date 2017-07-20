#ifndef __LOGGER_H_
#define __LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_LOGGER        1

#ifdef DEBUG_LOGGER
void log_init(const char * ip);
void log_deinit(void);
void log_print(const char *str);
void log_printf(const char *format, ...);
#else
#define log_init(x)
#define log_deinit()
#define log_print(x)
#define log_printf(x, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif
