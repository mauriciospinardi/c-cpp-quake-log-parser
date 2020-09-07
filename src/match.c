/**
 * @file match.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief LOG public API.
 * @date 2020-09-06
 * 
 */

#include "match.h"

#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********/
/* Macros */
/**********/

#define PARSER_KEY_KILL "Kill:"

/********************/
/* Global variables */
/********************/

static sem_t semaphore;

/***********************/
/* Function prototypes */
/***********************/

static int
import(const char *stream, unsigned long size, ST_MATCH **data);

/********************/
/* Public functions */
/********************/

/**
 * @brief Evaluates the content of a previously imported match stream.
 * 
 * @param[in,out] data match file structure
 * 
 * @return int ERR_xxx
 */
extern int
MATCH_evaluate(ST_MATCH **data)
{
    APPLICATION_TRACE("data [%lu]", data);

#warning TODO: MATCH_evaluate()

    (void) data;

    return ERR_INVALID_ARGUMENT;
}

/**
 * @brief Imports a match stream.
 *
 * @param[in] stream match buffer
 * @param[in] size size of match stream
 * @param[out] data match stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
MATCH_import(const char *stream, unsigned long size, ST_MATCH **data)
{
    int ret;

    APPLICATION_TRACE("*stream [%.32s...], size [%lu], data [%lu]", (stream) ? stream : "(null)", size, data);

    sem_wait(&semaphore);

    ret = import(stream, size, data);

    APPLICATION_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
}

/**
 * @brief Initializes the MATCH API.
 *
 * @return int ERR_NONE or ERR_xxx
 */
extern int
MATCH_start(void)
{
    static int start = -1;

    if (!start)
    {
        return ERR_ALREADY_STARTED;
    }

    sem_init(&semaphore, 0, 1);

    return ERR_NONE;
}

/*********************/
/* Private functions */
/*********************/

/**
 * @brief @ref MATCH_import()
 * 
 * @param[in] stream match buffer
 * @param[in] size size of match stream
 * @param[out] data match stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
static int
import(const char *stream, unsigned long size, ST_MATCH **data)
{
    if (!stream || (size <= 0))
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    (*data) = (ST_MATCH *) malloc(sizeof(ST_MATCH));

    if (!(*data))
    {
        return ERR_OUT_OF_MEMORY;
    }

    (*data)->buffer = (char *) malloc(sizeof(char) * (size + 1));

    if (!((*data)->buffer))
    {
        free(*data);

        return ERR_OUT_OF_MEMORY;
    }

    (*data)->next = NULL;

    memcpy((*data)->buffer, stream, size);

    (*data)->buffer[size] = 0;

    return ERR_NONE;
}
