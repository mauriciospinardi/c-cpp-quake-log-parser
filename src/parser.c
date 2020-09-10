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

typedef struct PLAYER_REPORT
{
    char *name;
    int killCount;
    struct PLAYER_REPORT *next;
} ST_PLAYER_REPORT;

typedef struct
{
    ST_PLAYER_REPORT *player;
    int killCount;
} ST_MATCH_REPORT;

typedef struct
{
    int matchCount;
    ST_MATCH_REPORT *matchReport; /* x matchCount */
} ST_REPORT;

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
#if 0

    ST_KILL *kill;
    ST_MATCH *match;
    ST_REPORT report;
    char *buffer;
    int index;

    if (!data)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!(data->log.match))
    {
        return ERR_INVALID_ARGUMENT;
    }

    match = data->log.match;

    memset(&report, 0, sizeof(ST_REPORT));

    while (match)
    {
        report.matchCount += 1;

        match = match->next;
    }

    report.matchReport = (ST_MATCH_REPORT *) malloc(sizeof(ST_MATCH_REPORT) * report.matchCount);

    if (!report.matchReport)
    {
        return ERR_OUT_OF_MEMORY;
    }

    memset(report.matchReport, 0, sizeof(ST_MATCH_REPORT) * report.matchCount);

    index = 0;

    match = data->log.match;

    while (match)
    {
        kill = match->kill;

        while (kill)
        {
            if (!kill->buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer = strstr(kill->buffer + strlen(PARSER_KEY_KILL) + 1, ":");

            if (!buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer += 2; /* ": " */

            if (!strcmp("<world>", buffer))
            {
                /* TODO: (A) find/add player and (B) substract 1 from his kill
                 * count */
            }
            else
            {
                /* TODO: (A) find/add player and (B) add 1 to his kill count */
            }

            APPLICATION_TRACE("kill [%.16s...]", buffer);

            report.matchReport[index].killCount += 1;

            kill = kill->next;
        }

        APPLICATION_TRACE("report.matchReport[%d].killCount [%d]", index, report.matchReport[index].killCount);

        match = match->next;

        index += 1;
    }

    APPLICATION_TRACE("report.matchCount [%d]", report.matchCount);

#else

#warning TODO: PARSER_report()

    (void) data;

#endif /* #if 0 */

    return ERR_DEFAULT;
}
