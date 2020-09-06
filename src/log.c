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

/********************/
/* Global variables */
/********************/

static sem_t semaphore;

/***********************/
/* Function prototypes */
/***********************/

static int
fsize(const char *file, long int *size);

static int
import(const char *file, ST_PARSER *log);

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref log.h
 * 
 * @param[in] file file name
 * @param[out] log log file structure
 * 
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
extern int API_PUBLIC
LOG_import(const char *file, ST_PARSER *log)
{
    int ret;

    PARSER_TRACE("*file [%s], log [%lu]", (file) ? file : "(null)", log);

    sem_wait(&semaphore);

    ret = import(file, log);

    PARSER_TRACE("ret [%d]", ret);

    sem_post(&semaphore);

    return ret;
}

/**
 * @brief @ref log.h
 * 
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
int
LOG_start(void)
{
    static int start = -1;

    if (!start)
    {
        return LOG_ERR_ALREADY_STARTED;
    }

    sem_init(&semaphore, 0, 1);

    return LOG_SUCCESS;
}

/*********************/
/* Private functions */
/*********************/

/**
 * @brief File size designator. Returns the file size.
 * Inherently not thread safe. Should be called only within import().
 * 
 * @param[in] file file name
 * @param[out] size file size
 * 
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
static int
fsize(const char *file, long int *size)
{
    FILE *fp;

    if (!file || !size)
    {
        return LOG_ERR_INVALID_ARGUMENT;
    }

    fp = fopen(file, "r");

    if (!fp)
    {
        switch (errno)
        {
        case ENOENT:
            return LOG_ERR_FILE_NOT_FOUND;

        default:
            return LOG_ERR_DEFAULT;
        }
    }

    fseek(fp, 0, SEEK_END);

    *size = ftell(fp);

    fseek(fp, 0, SEEK_SET);

    fclose(fp);

    if (*size < 0)
    {
        return LOG_ERR_DEFAULT;
    }

    return LOG_SUCCESS;
}

/**
 * @brief @ref LOG_import()
 * 
 * @param[in] file file name
 * @param[out] log log file structure
 * 
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
static int
import(const char *file, ST_PARSER *log)
{
    FILE *fp;
    int ret;
    long int size;

    if (!file || !log)
    {
        return LOG_ERR_INVALID_ARGUMENT;
    }

    log->file = (char *) malloc(sizeof(char) * (strlen(file) + 1));

    if (!log->file)
    {
        return LOG_ERR_OUT_OF_MEMORY;
    }

    strcpy(log->file, file);

    ret = fsize(log->file, &size);

    if (ret)
    {
        return ret;
    }

    if (!size)
    {
        return LOG_ERR_FILE_EMPTY;
    }

    log->buffer = (char *) malloc(sizeof(char) * (size + 1));

    if (!log->buffer)
    {
        return LOG_ERR_OUT_OF_MEMORY;
    }

    fp = fopen(log->file, "r");

    if (!fp)
    {
        switch (errno)
        {
        case ENOENT:
            return LOG_ERR_FILE_NOT_FOUND;

        default:
            return LOG_ERR_DEFAULT;
        }
    }

    fseek(fp, 0, SEEK_SET);

    ret = fread(log->buffer, sizeof(char), size, fp);

    fclose(fp);

    if (ret != size)
    {
        return LOG_ERR_DEFAULT;
    }

    return LOG_SUCCESS;
}
