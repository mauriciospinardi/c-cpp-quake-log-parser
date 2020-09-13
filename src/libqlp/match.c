/**
 * @file match.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief MATCH API.
 * @date 2020-09-06
 * 
 */

#include "match.h"

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
evaluate(ST_MATCH **data);

static int
import(const char *stream, unsigned long size, ST_MATCH **data);

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref match.h
 * 
 * @param[in,out] data match file structure
 * 
 * @return int ERR_xxx
 */
extern int
MATCH_evaluate(ST_MATCH **data)
{
    int retValue;

    LIBQLP_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = evaluate(data);
    }

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref match.h
 *
 * @param[in] stream match buffer
 * @param[in] size size of match stream
 * @param[out] data match stream structure
 * 
 * @return int ERR_xxx
 */
extern int
MATCH_import(const char *stream, unsigned long size, ST_MATCH **data)
{
    int retValue;

    LIBQLP_TRACE("*stream [%.32s...], size [%lu], data [%lu]", (stream) ? stream : "(null)", size, data);

    sem_wait(&semaphore);

    retValue = import(stream, size, data);

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref match.h
 *
 * @return int ERR_xxx
 */
extern int
MATCH_start(void)
{
    static int start = -1;

    if (!start)
    {
        return ERR_ALREADY_STARTED;
    }

    if (KILL_start())
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
 * @brief @ref MATCH_evaluate()
 * 
 * @param data match file structure
 * 
 * @return int ERR_xxx
 */
static int
evaluate(ST_MATCH **data)
{
    ST_KILL **kill;
    char *begin;
    char *end;
    int retValue;
    unsigned long size;

    if (!data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!(*data)->buffer)
    {
        return ERR_INVALID_ARGUMENT;
    }

    kill = &((*data)->kill);

    begin = (*data)->buffer;

    while (begin)
    {
        begin = strstr(begin, QLP_KEY_KILL);

        if (!begin)
        {
            break;
        }

        end = strstr(begin + strlen(QLP_KEY_KILL), QLP_KEY_KILL);

        if (!end)
        {
            end = begin;
        }

        size = strlen(begin) - strlen(end);

        if (!size)
        {
            size = strlen(begin);
        }

        retValue = KILL_import(begin, size, kill);

        if (retValue)
        {
            return retValue;
        }

        kill = &(*kill)->next;

        if (begin != end)
        {
            begin = end;
        }
        else
        {
            begin = NULL; /* EOF */
        }
    }

    return ERR_NONE; /* Kills aren't mandatory */
}

/**
 * @brief @ref MATCH_import()
 * 
 * @param stream match buffer
 * @param size size of match stream
 * @param data match stream structure
 * 
 * @return int ERR_xxx
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

    (*data)->kill = NULL;

    memcpy((*data)->buffer, stream, size);

    (*data)->buffer[size] = 0;

    return ERR_NONE;
}
