#include <features.h>
#ifndef __LOGGER_H__
#define __LOGGER_H__
__BEGIN_DECLS

void logger_trace(const char* msg, ...);
void logger_debug(const char* msg, ...);
void logger_info(const char* msg, ...);
void logger_warn(const char* msg, ...);
void logger_error(const char* msg, ...);
void logger_fatal(const char* msg, ...);

__END_DECLS
#endif // __LOGGER_H__
