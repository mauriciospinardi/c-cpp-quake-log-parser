/**
 * @file parser.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief PARSER public API.
 * @date 2020-09-03
 * 
 */

#include "parser.h"

#include "cJSON/1.7.14/cJSON.h"

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
createJSON(ST_REPORT *report, cJSON **json);

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

    if (!strncmp(name, PARSER_KEY_WORLD, lenght))
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
 * @brief Creates a JSON stream from a ST_REPORT structure.
 * 
 * @param report ST_REPORT structure
 * @param json cJSON stream
 * 
 * @return int ERR_xxx
 */
static int
createJSON(ST_REPORT *report, cJSON **json)
{
    ST_PLAYER_REPORT *player;
    cJSON *item[2];
    cJSON *array;
    cJSON *object[3];
    char matchID[16];
    int index;

    if (!report || !json)
    {
        return ERR_INVALID_ARGUMENT;
    }

    for (index = 0; index < report->matchCount; index++)
    {
        object[1] = cJSON_CreateObject();

        if (!object[1])
        {
            return ERR_OUT_OF_MEMORY;
        }

        item[0] = cJSON_CreateNumber((double) report->matchReport[index].killCount);

        if (!item[0])
        {
            cJSON_Delete(object[1]);

            return ERR_OUT_OF_MEMORY;
        }

        cJSON_AddItemToObject(object[1], "total_kills", item[0]);

        object[0] = cJSON_CreateObject();

        if (!object[0])
        {
            cJSON_Delete(item[0]); cJSON_Delete(object[1]);

            return ERR_OUT_OF_MEMORY;
        }

        sprintf(matchID, "game_%d", index + 1);

        player = report->matchReport[index].player;

        array = cJSON_CreateArray();

        if (!array)
        {
            cJSON_Delete(object[0]); cJSON_Delete(item[0]); cJSON_Delete(object[1]);

            return ERR_OUT_OF_MEMORY;
        }

        object[2] = cJSON_CreateObject();
        
        if (!object[2])
        {
            cJSON_Delete(array); cJSON_Delete(object[0]); cJSON_Delete(item[0]);

            cJSON_Delete(object[1]);

            return ERR_OUT_OF_MEMORY;
        }

        while (player)
        {
            item[0] = cJSON_CreateString(player->name);

            if (!item[0])
            {
                cJSON_Delete(object[2]); cJSON_Delete(array); cJSON_Delete(object[0]);

                cJSON_Delete(item[0]); cJSON_Delete(object[1]);

                return ERR_OUT_OF_MEMORY;
            }

            cJSON_AddItemToArray(array, item[0]);

            item [1] = cJSON_CreateNumber(player->killCount);

            if (!item[1])
            {
                cJSON_Delete(item[0]); cJSON_Delete(object[2]); cJSON_Delete(array); cJSON_Delete(object[0]);

                cJSON_Delete(item[0]); cJSON_Delete(object[1]);

                return ERR_OUT_OF_MEMORY;
            }

            cJSON_AddItemToObject(object[2], player->name, item[1]);

            player = player->next;
        }

        cJSON_AddItemToObject(object[1], "players", array);

        cJSON_AddItemToObject(object[1], "kills", object[2]);

        cJSON_AddItemToObject(object[0], matchID, object[1]);
    }

    *json = object[0];

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
    cJSON *json;
    char *buffer;
    int index;
    int retValue;
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

    json = NULL;

    index = 0;

    match = data->log.match;

    while (match)
    {
        buffer = match->buffer;

        while (buffer)
        {
            buffer = strstr(buffer, PARSER_KEY_PLAYER);

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

            if (strncmp(PARSER_KEY_WORLD, buffer, lenght))
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

    retValue = createJSON(&report, &json);

    if (retValue)
    {
        return ERR_INVALID_ARGUMENT;
    }

    /* TODO:
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

    if (!strncmp(name, PARSER_KEY_WORLD, lenght))
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
