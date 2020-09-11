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
appendPlayer(ST_PLAYER_REPORT **list, char *name, int lenght);

static int
report(ST_PARSER *data);

static int
updatePlayer(ST_PLAYER_REPORT *list, char *name, int lenght, int count);

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
 * @brief Appends a player to a player list.
 * 
 * @param list player list
 * @param name player name
 * @param lenght lenght of player name
 * 
 * @return int ERR_xxx
 */
static int
appendPlayer(ST_PLAYER_REPORT **list, char *name, int lenght)
{
    ST_PLAYER_REPORT *pointer;

    if (!list || !name || lenght <= 0)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!strncmp(name, "<world>", lenght))
    {
        return ERR_INVALID_ARGUMENT;
    }

    pointer = *list;

    while (pointer)
    {
        if (!strncmp(pointer->name, name, lenght)) /* Player already on the list */
        {
            return ERR_NONE;
        }

        pointer = pointer->next;
    }

    pointer = (ST_PLAYER_REPORT *) malloc(sizeof(ST_PLAYER_REPORT));

    if (!pointer)
    {
        return ERR_OUT_OF_MEMORY;
    }

    pointer->next = *list;

    pointer->killCount = 0;

    pointer->name = (char *) malloc(sizeof(char) * (lenght + 1));

    if (!pointer->name)
    {
        free(pointer);

        return ERR_OUT_OF_MEMORY;
    }

    strncpy(pointer->name, name, lenght);

    pointer->name[lenght] = 0;

    *list = pointer;

    return ERR_NONE;
}

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
    ST_KILL *kill;
    ST_MATCH *match;
    ST_REPORT report;
    char *buffer;
    int index;
    unsigned long lenght;

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
        buffer = match->buffer;

        while (buffer)
        {
            buffer = strstr(buffer, "ClientUserinfoChanged:");

            if (!buffer) /* No players found */
            {
                break;
            }

            buffer = strstr(buffer, "n\\");

            if (!buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer += 2; /* "n\" */

            lenght = (unsigned long) strstr(buffer, "\\t");

            if (!lenght)
            {
                return ERR_INVALID_ARGUMENT;
            }

            lenght -= (unsigned long) buffer;

            appendPlayer(&(report.matchReport[index].player), buffer, (int) lenght);
        }

        kill = match->kill;

        while (kill)
        {
            if (!kill->buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer = kill->buffer;

            buffer = strstr(buffer + strlen(PARSER_KEY_KILL), ":");

            if (!buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer += 2; /* ": " */

            lenght = (unsigned long) strstr(buffer, " killed");

            if (!lenght)
            {
                return ERR_INVALID_ARGUMENT;
            }

            lenght -= (unsigned long) buffer;

            if (strncmp("<world>", buffer, lenght))
            {
                updatePlayer(report.matchReport[index].player, buffer, (int) lenght, 1);
            }
            else
            {
                buffer = strstr(buffer, "killed ") + strlen("killed ");

                lenght = (unsigned long) strstr(buffer, " by");

                if (!lenght)
                {
                    return ERR_INVALID_ARGUMENT;
                }

                lenght -= (unsigned long) buffer;

                updatePlayer(report.matchReport[index].player, buffer, (int) lenght, -1);
            }

            report.matchReport[index].killCount += 1;

            kill = kill->next;
        }

        index += 1;

        match = match->next;
    }

    /* TODO:
     * - createJSON()
     * - saveReport()
     */

    return ERR_DEFAULT;
}

/**
 * @brief Updates the kill count of a player.
 * 
 * @param list player list
 * @param name player name
 * @param lenght lenght of player name
 * @param count value to be added to the player kill count
 * 
 * @return int ERR_xxx
 */
static int
updatePlayer(ST_PLAYER_REPORT *list, char *name, int lenght, int count)
{
    if (!list || !name || lenght <= 0)
    {
        return ERR_INVALID_ARGUMENT;
    }

    if (!strncmp(name, "<world>", lenght))
    {
        return ERR_INVALID_ARGUMENT;
    }

    while (list)
    {
        if (strncmp(list->name, name, lenght)) /* Player found */
        {
            list = list->next;

            continue;
        }

        list->killCount += count;

        return ERR_NONE;
    }

    return ERR_INVALID_ARGUMENT;
}
