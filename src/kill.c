/**
 * @file kill.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief KILL API.
 * @date 2020-09-10
 * 
 */

#include "kill.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************/
/* Global variables */
/********************/

static sem_t semaphore;

/***********************/
/* Function prototypes */
/***********************/

static int
import(const char *stream, unsigned long size, ST_KILL **data);

/********************/
/* Public functions */
/********************/

/**
 * @brief Imports a kill stream.
 *
 * @param[in] stream kill buffer
 * @param[in] size size of kill stream
 * @param[out] data kill stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
KILL_import(const char *stream, unsigned long size, ST_KILL **data)
{
    int retValue;

    APPLICATION_TRACE("*stream [%.32s...], size [%lu], data [%lu]", (stream) ? stream : "(null)", size, data);

    sem_wait(&semaphore);

    retValue = import(stream, size, data);

    APPLICATION_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief Initializes the KILL API.
 *
 * @return int ERR_NONE or ERR_xxx
 */
extern int
KILL_start(void)
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
 * @brief @ref KILL_import()
 * 
 * @param[in] stream kill buffer
 * @param[in] size size of kill stream
 * @param[out] data kill stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
static int
import(const char *stream, unsigned long size, ST_KILL **data)
{
    if (!stream || (size <= 0))
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    (*data) = (ST_KILL *) malloc(sizeof(ST_KILL));

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
