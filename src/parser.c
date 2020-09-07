/**
 * @file parser.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief PARSER public API.
 * @date 2020-09-03
 * 
 */

#include "parser.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************/
/* Global variables */
/********************/

static sem_t semaphore;

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref parser.h
 * 
 * @param[in,out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_evaluate(ST_PARSER *data)
{
    int ret;

    APPLICATION_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    ret = ERR_INVALID_ARGUMENT;

    if (data)
    {
        ret = LOG_evaluate(&data->log);
    }

    APPLICATION_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
}

/**
 * @brief @ref parser.h
 *
 * @param[in] file file name
 * @param[out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_import(const char *file, ST_PARSER *data)
{
    int ret;

    APPLICATION_TRACE("*file [%s], data [%lu]", (file) ? file : "(null)", data);

    sem_wait(&semaphore);

    ret = ERR_INVALID_ARGUMENT;

    if (data)
    {
        ret = LOG_import(file, &data->log);
    }

    APPLICATION_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
}

/**
 * @brief @ref parser.h
 * 
 * @param[in] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_report(ST_PARSER *data)
{

#warning TODO: PARSER_report()

    (void) data;

    return ERR_DEFAULT;
}

/**
 * @brief @ref parser.h
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_start(void)
{
    static int start = -1;

    if (!start)
    {
        return ERR_ALREADY_STARTED;
    }

    if (UTILITIES_start())
    {
        return ERR_DEFAULT;
    }

    if (LOG_start())
    {
        return ERR_DEFAULT;
    }

    sem_init(&semaphore, 0, 1);

    return ERR_NONE;
}
