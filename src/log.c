/**
 * @file log.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief LOG public API.
 * @date 2020-09-06
 * 
 */

#include "log.h"

#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********/
/* Macros */
/**********/

#define PARSER_KEY_MATCH "InitGame:"

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
fsize(const char *file, long int *size);

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
    int ret;

    APPLICATION_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    ret = ERR_INVALID_ARGUMENT;

    if (data)
    {
        ret = evaluate(data);
    }

    APPLICATION_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
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
    int ret;

    APPLICATION_TRACE("*file [%s], data [%lu]", (file) ? file : "(null)", data);

    sem_wait(&semaphore);

    ret = import(file, data);

    APPLICATION_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
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
    int ret;
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
        begin = strstr(begin, PARSER_KEY_MATCH);

        if (!begin)
        {
            break;
        }

        end = strstr(begin + strlen(PARSER_KEY_MATCH), PARSER_KEY_MATCH);

        if (!end)
        {
            end = begin;
        }

        size = strlen(begin) - strlen(end);

        if (!size)
        {
            size = strlen(begin);
        }

        ret = MATCH_import(begin, size, match);

        if (*match)
        {
            match = &(*match)->next;
        }
        else
        {
            return ERR_OUT_OF_MEMORY;
        }

        if (begin != end)
        {
            begin = end;
        }
        else
        {
            begin = NULL; /* EOF */
        }
    }

    if (!data->match)
    {
        return ERR_MATCH_NOT_FOUND;
    }

    match = &data->match;

    while (*match)
    {
        ret = MATCH_evaluate(match);

        if (ret)
        {
            return ret;
        }

        match = &(*match)->next;
    }

    return ERR_NONE;
}

/**
 * @brief Returns a file size. Inherently not thread safe.
 *
 * @param[in] file file name
 * @param[out] size file size
 *
 * @return int ERR_NONE or ERR_xxx
 */
static int
fsize(const char *file, long int *size)
{
    FILE *fp;

    if (!file || !size)
    {
        return ERR_INVALID_ARGUMENT;
    }

    fp = fopen(file, "r");

    if (!fp)
    {
        switch (errno)
        {
        case ENOENT:
            return ERR_FILE_NOT_FOUND;

        default:
            return ERR_DEFAULT;
        }
    }

    fseek(fp, 0, SEEK_END);

    *size = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    fclose(fp);

    if (*size < 0)
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
    FILE *fp;
    int ret;
    long int size;

    if (!file || !data)
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

    ret = fsize(data->file, &size);

    if (ret)
    {
        free(data->file);

        return ret;
    }

    if (!size)
    {
        free(data->file);

        return ERR_FILE_EMPTY;
    }

    data->buffer = (char *) malloc(sizeof(char) * (size + 1));

    if (!data->buffer)
    {
        free(data->file); free(data->buffer);

        return ERR_OUT_OF_MEMORY;
    }

    fp = fopen(data->file, "r");

    if (!fp)
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

    fseek(fp, 0, SEEK_SET);

    ret = fread(data->buffer, sizeof(char), size, fp);

    fclose(fp);

    if (ret != size)
    {
        return ERR_DEFAULT; /* Partial reading (mem. is kept allocated for further analysis) */
    }

    return ERR_NONE;
}
