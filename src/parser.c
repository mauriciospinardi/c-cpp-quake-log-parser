/**
 * @file parser.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Main Quake Log Parser API.
 * @date 2020-09-03
 * 
 */

#include "parser.h"

#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref parser.h
 * 
 * @param[in] format autodescriptive
 * @param[in] ... variable number of arguments
 */
extern void API_PUBLIC
PARSER_log(const char *date, const char *time, const char *file, const int line, const char *function, const char *format, ...)
{
    char log[256];
    int ret;
    va_list args;

    static sem_t *semaphore = NULL;

    if (!format)
    {
        return;
    }

    if (!semaphore) /* Safe "enough", but not entirely */
    {
        semaphore = (sem_t *) malloc(sizeof(sem_t));

        if (!semaphore)
        {
            return;
        }

        sem_init(semaphore, 0, 1);
    }

    sem_wait(semaphore);

    va_start(args, format);

    printf("\r\n[DEBUG] %11.11s %8.8s %.17s #%.4d %s::", date, time, file, line, function);

    ret = vsnprintf(log, sizeof(log), format, args);

    if (ret)
    {
        printf("%s", log);
    }

    va_end(args);

    sem_post(semaphore);
}
