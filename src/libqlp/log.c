/**
 * @file log.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief LOG API.
 * @date 2020-09-06
 * 
 */

#include "log.h"

#include <errno.h>
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
evaluate(ST_LOG *data);

static int
fsize(const char *file, long int *fileSize);

static int
import(const char *file, ST_LOG *data);

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref log.h
 * 
 * @param[in,out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
LOG_evaluate(ST_LOG *data)
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
 * @brief @ref log.h
 * 
 * @param[in] file file name
 * @param[out] data log file structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
LOG_import(const char *file, ST_LOG *data)
{
    int retValue;

    LIBQLP_TRACE("*file [%s], data [%lu]", (file) ? file : "(null)", data);

    sem_wait(&semaphore);

    retValue = import(file, data);

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref log.h
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
LOG_start(void)
{
    static int start = -1;

    if (!start)
    {
        return ERR_ALREADY_STARTED;
    }

    if (MATCH_start())
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
 * @brief @ref LOG_evaluate()
 * 
 * @param[in,out] data log file structure
 * 
 * @return int ERR_xxx
 */
static int
evaluate(ST_LOG *data)
{
    ST_MATCH **match;
    char *begin;
    char *end;
    int retValue;
    unsigned long size;

    if (!data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!data->file || !data->buffer)
    {
        return ERR_INVALID_ARGUMENT;
    }

    match = &data->match;

    begin = data->buffer;

    while (begin)
    {
        begin = strstr(begin, QLP_KEY_MATCH);

        if (!begin)
        {
            break;
        }

        end = strstr(begin + strlen(QLP_KEY_MATCH), QLP_KEY_MATCH);

        if (!end)
        {
            end = begin;
        }

        size = strlen(begin) - strlen(end);

        if (!size)
        {
            size = strlen(begin);
        }

        retValue = MATCH_import(begin, size, match);

        if (retValue)
        {
            return retValue;
        }

        retValue = MATCH_evaluate(match);

        if (retValue)
        {
            return retValue;
        }

        match = &(*match)->next;

        if (begin != end)
        {
            begin = end;
        }
        else
        {
            begin = NULL; /* EOF */
        }
    }

    return (!data->match) ? ERR_MATCH_NOT_FOUND : ERR_NONE;
}

/**
 * @brief Returns a file size. Inherently not thread safe.
 *
 * @param[in] file file name
 * @param[out] fileSize file size
 *
 * @return int ERR_NONE or ERR_xxx
 */
static int
fsize(const char *file, long int *fileSize)
{
    FILE *filePointer;

    if (!file || !fileSize)
    {
        return ERR_INVALID_ARGUMENT;
    }

    filePointer = fopen(file, "r");

    if (!filePointer)
    {
        switch (errno)
        {
        case ENOENT:
            return ERR_FILE_NOT_FOUND;

        default:
            return ERR_DEFAULT;
        }
    }

    fseek(filePointer, 0, SEEK_END);

    *fileSize = ftell(filePointer);

    fseek(filePointer, 0, SEEK_SET);

    fclose(filePointer);

    if (*fileSize < 0)
    {
        return ERR_DEFAULT;
    }

    return ERR_NONE;
}

/**
 * @brief @ref LOG_import()
 * 
 * @param[in] file file name
 * @param[out] data log file structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
static int
import(const char *file, ST_LOG *data)
{
    FILE *filePointer;
    int retValue;
    long int fileSize;

    if (!file || !data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!file[0])
    {
        return ERR_INVALID_ARGUMENT;
    }

    memset(data, 0, sizeof(ST_LOG));

    data->file = (char *) malloc(sizeof(char) * (strlen(file) + 1));

    if (!data->file)
    {
        return ERR_OUT_OF_MEMORY;
    }

    strcpy(data->file, file);

    retValue = fsize(data->file, &fileSize);

    if (retValue)
    {
        free(data->file);

        return retValue;
    }

    if (!fileSize)
    {
        free(data->file);

        return ERR_FILE_EMPTY;
    }

    data->buffer = (char *) malloc(sizeof(char) * (fileSize + 1));

    if (!data->buffer)
    {
        free(data->file); free(data->buffer);

        return ERR_OUT_OF_MEMORY;
    }

    filePointer = fopen(data->file, "r");

    if (!filePointer)
    {
        free(data->file); free(data->buffer);

        switch (errno)
        {
        case ENOENT:
            return ERR_FILE_NOT_FOUND;

        default:
            return ERR_INVALID_ARGUMENT;
        }
    }

    fseek(filePointer, 0, SEEK_SET);

    retValue = fread(data->buffer, sizeof(char), fileSize, filePointer);

    fclose(filePointer);

    if (retValue != fileSize)
    {
        return ERR_DEFAULT; /* Partial reading (mem. is kept allocated for further analysis) */
    }

    return ERR_NONE;
}
