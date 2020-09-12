/**
 * @file utilities.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief UTILITIES API.
 * @date 2020-09-03
 * 
 */

#include "utilities.h"

#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/********************/
/* Global variables */
/********************/

static sem_t trace_semaphore;

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref utilities.h
 * 
 * @param[in] format autodescriptive
 * @param[in] ... variable number of arguments
 */
extern void
UTILITIES_log(const char *date, const char *time, const char *file, const int line, const char *function, const char *format, ...)
{
    char log[256];
    int retValue;
    va_list args;

    if (!format)
    {
        return;
    }

    sem_wait(&trace_semaphore);

    va_start(args, format);

    printf("\r\n[DEBUG] %11.11s %8.8s %.17s #%.4d %s::", date, time, file, line, function);

    retValue = vsnprintf(log, sizeof(log), format, args);

    if (retValue)
    {
        printf("%s", log);
    }

    va_end(args);

    sem_post(&trace_semaphore);
}

/**
 * @brief @ref utilities.h
 * 
 * @return int ERR_xxx
 */
extern int
UTILITIES_start(void)
{
    static int start = -1;

    if (!start)
    {
        return ERR_ALREADY_STARTED;
    }

    sem_init(&trace_semaphore, 0, 1);

    return ERR_NONE;
}
