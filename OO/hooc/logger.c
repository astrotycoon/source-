#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

void logger_trace(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;34mTrace:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}

void logger_debug(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;37mDebug:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}

void logger_info(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;32mInfo:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}

void logger_warn(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;33mWarning:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}

void logger_error(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;31mError:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}

void logger_fatal(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    printf("\033[01;31mFatal:\033[00m ");
    vprintf(msg, ap);
    printf("\n");
    va_end(ap);
    return;
}
