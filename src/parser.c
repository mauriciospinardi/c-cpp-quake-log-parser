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
/* Type definitions */
/********************/

/********************/
/* Global variables */
/********************/

static sem_t semaphore;

/***********************/
/* Function prototypes */
/***********************/

static int
report(ST_PARSER *data);

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref parser.h
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_evaluate(ST_PARSER *data)
{
    int retValue;

    APPLICATION_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = LOG_evaluate(&data->log);
    }

    APPLICATION_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref parser.h
 *
 * @param[in] file file name
 * @param[out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_import(const char *file, ST_PARSER *data)
{
    int retValue;

    APPLICATION_TRACE("*file [%s], data [%lu]", (file) ? file : "(null)", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = LOG_import(file, &data->log);
    }

    APPLICATION_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref parser.h
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_report(ST_PARSER *data)
{
    int retValue;

    APPLICATION_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = report(data);
    }

    APPLICATION_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
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

/*********************/
/* Private functions */
/*********************/

/**
 * @brief @ref PARSER_report()
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
static int
report(ST_PARSER *data)
{

#warning TODO: PARSER_report()

    (void) data;

    return ERR_DEFAULT;
}
