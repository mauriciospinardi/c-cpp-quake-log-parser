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
/* Global variables */
/********************/

static sem_t semaphore;

static sem_t trace;

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref parser.h
 * 
 * @param[in,out] log log file structure
 * 
 * @return int PAR_SUCCESS or xxx_ERR_xxx
 */
extern int
PARSER_evaluate(ST_PARSER *log)
{
    /* TODO: code */

    (void) log;

    return PAR_ERR_DEFAULT;
}

/**
 * @brief @ref parser.h
 *
 * @param[in] file file name
 * @param[out] log log file structure
 * 
 * @return int PAR_SUCCESS or xxx_ERR_xxx
 */
extern int
PARSER_import(const char *file, ST_PARSER *log)
{
    int ret;

    PARSER_TRACE("*file [%s], log [%lu]", (file) ? file : "(null)", log);

    sem_wait(&semaphore);

    ret = LOG_import(file, log);

    PARSER_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
}

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

    if (!format)
    {
        return;
    }

    sem_wait(&trace);

    va_start(args, format);

    printf("\r\n[DEBUG] %11.11s %8.8s %.17s #%.4d %s::", date, time, file, line, function);

    ret = vsnprintf(log, sizeof(log), format, args);

    if (ret)
    {
        printf("%s", log);
    }

    va_end(args);

    sem_post(&trace);
}

/**
 * @brief @ref parser.h
 * 
 * @param[in] log log file structure
 * 
 * @return int PAR_SUCCESS or xxx_ERR_xxx
 */
extern int
PARSER_report(ST_PARSER *log)
{
    /* TODO: code */

    (void) log;

    return PAR_ERR_DEFAULT;
}

/**
 * @brief @ref parser.h
 * 
 * @return int PAR_SUCCESS or xxx_ERR_xxx
 */
int
PARSER_start(void)
{
    static int start = -1;

    if (!start)
    {
        return PAR_ERR_ALREADY_STARTED;
    }

    if (LOG_start())
    {
        return PAR_ERR_DEFAULT;
    }

    sem_init(&semaphore, 0, 1);

    sem_init(&trace, 0, 1);

    return PAR_SUCCESS;
}
