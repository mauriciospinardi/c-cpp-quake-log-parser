/**
 * @file parser.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief QLP public API.
 * @date 2020-09-03
 * 
 */

#include "qlp.h"

#include "libcjson/cJSON.h"

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********/
/* Macros */
/**********/

#define MEANS_OF_DEATH_LIST_SIZE ((int) (sizeof(ST_KILL_MEAN) * TOTAL_MEANS_OF_DEATH))

#define TOTAL_MEANS_OF_DEATH ((int) (sizeof(meansOfDeath) / sizeof(ST_KILL_MEAN)))

/********************/
/* Type definitions */
/********************/

typedef struct KILL_MEAN
{
    char name[32];
    int killCount;
} ST_KILL_MEAN;

typedef struct PLAYER_REPORT
{
    char *name;
    int killCount;
    struct PLAYER_REPORT *next;
} ST_PLAYER_REPORT;

typedef struct
{
    ST_KILL_MEAN *meanOfDeath;
    ST_PLAYER_REPORT *player;
    int killCount;
} ST_MATCH_REPORT;

typedef struct
{
    int matchCount;
    ST_MATCH_REPORT *matchReport;
} ST_REPORT;

/**************/
/* Constantes */
/**************/

static const ST_KILL_MEAN meansOfDeath[] =
{
    { "MOD_UNKNOWN", 0 },
    { "MOD_SHOTGUN", 0 },
    { "MOD_GAUNTLET", 0 },
    { "MOD_MACHINEGUN", 0 },
    { "MOD_GRENADE", 0 },
    { "MOD_GRENADE_SPLASH", 0 },
    { "MOD_ROCKET", 0 },
    { "MOD_ROCKET_SPLASH", 0 },
    { "MOD_PLASMA", 0 },
    { "MOD_PLASMA_SPLASH", 0 },
    { "MOD_RAILGUN", 0 },
    { "MOD_LIGHTNING", 0 },
    { "MOD_BFG", 0 },
    { "MOD_BFG_SPLASH", 0 },
    { "MOD_WATER", 0 },
    { "MOD_SLIME", 0 },
    { "MOD_LAVA", 0 },
    { "MOD_CRUSH", 0 },
    { "MOD_TELEFRAG", 0 },
    { "MOD_FALLING", 0 },
    { "MOD_SUICIDE", 0 },
    { "MOD_TARGET_LASER", 0 },
    { "MOD_TRIGGER_HURT", 0 },
    { "MOD_NAIL", 0 },
    { "MOD_CHAINGUN", 0 },
    { "MOD_PROXIMITY_MINE", 0 },
    { "MOD_KAMIKAZE", 0 },
    { "MOD_JUICED", 0 },
    { "MOD_GRAPPLE", 0 }
};

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
report(ST_QLP *data);

static int
updateMeanOfDeath(ST_KILL_MEAN *list, char *name);

static int
updatePlayer(ST_PLAYER_REPORT *list, char *name, int lenght, int count);

/********************/
/* Public functions */
/********************/

/**
 * @brief @ref libqlp/qlp.h
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_evaluate(ST_QLP *data)
{
    int retValue;

    LIBQLP_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = LOG_evaluate(&data->log);
    }

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref libqlp/qlp.h
 *
 * @param[in] file file name
 * @param[out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_import(const char *file, ST_QLP *data)
{
    int retValue;

    LIBQLP_TRACE("*file [%s], data [%lu]", (file) ? file : "(null)", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = LOG_import(file, &data->log);
    }

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref libqlp/qlp.h
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_report(ST_QLP *data)
{
    int retValue;

    LIBQLP_TRACE("data [%lu]", data);

    sem_wait(&semaphore);

    retValue = ERR_INVALID_ARGUMENT;

    if (data)
    {
        retValue = report(data);
    }

    LIBQLP_TRACE("retValue [%d]", retValue);

    sem_post(&semaphore);

    return retValue;
}

/**
 * @brief @ref libqlp/qlp.h
 * 
 * @return int ERR_xxx
 */
extern int
QLP_start(void)
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

    if (!strncmp(name, QLP_KEY_WORLD_PLAYER, lenght))
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
        UTILITIES_abort();
    }

    pointer->next = *list;

    pointer->killCount = 0;

    pointer->name = (char *) malloc(sizeof(char) * (lenght + 1));

    if (!pointer->name)
    {
        UTILITIES_abort();
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
    cJSON *array;
    cJSON *item[2];
    cJSON *object[3];
    char *stream;
    char matchID[16];
    int i, j;
    int includeMeans;

    if (!report || !json)
    {
        return ERR_INVALID_ARGUMENT;
    }

    *json = cJSON_CreateObject();

    if (!*json)
    {
        UTILITIES_abort();
    }

    for (i = 0; i < report->matchCount; i++)
    {
        object[0] = cJSON_CreateObject();

        if (!object[0])
        {
            UTILITIES_abort();
        }

        item[0] = cJSON_CreateNumber((double) report->matchReport[i].killCount);

        if (!item[0])
        {
            UTILITIES_abort();
        }

        cJSON_AddItemToObject(object[0], "total_kills", item[0]);

        sprintf(matchID, "game_%d", i + 1);

        player = report->matchReport[i].player;

        array = cJSON_CreateArray();

        if (!array)
        {
            UTILITIES_abort();
        }

        object[1] = cJSON_CreateObject();
        
        if (!object[1])
        {
            UTILITIES_abort();
        }

        while (player)
        {
            item[0] = cJSON_CreateString(player->name);

            if (!item[0])
            {
                UTILITIES_abort();
            }

            cJSON_AddItemToArray(array, item[0]);

            item [1] = cJSON_CreateNumber(player->killCount);

            if (!item[1])
            {
                UTILITIES_abort();
            }

            cJSON_AddItemToObject(object[1], player->name, item[1]);

            player = player->next;
        }

        includeMeans = 0;

        object[2] = cJSON_CreateObject();

        for (j = (!object[2]) ? TOTAL_MEANS_OF_DEATH : 0; j < TOTAL_MEANS_OF_DEATH; j++)
        {
            if (!report->matchReport[i].meanOfDeath[j].killCount)
            {
                continue;
            }

            item[1] = cJSON_CreateNumber(report->matchReport[i].meanOfDeath[j].killCount);

            if (!item[1])
            {
                UTILITIES_abort();
            }

            cJSON_AddItemToObject(object[2], report->matchReport[i].meanOfDeath[j].name, item[1]);

            includeMeans = 1;
        }

        cJSON_AddItemToObject(object[0], "players", array);

        cJSON_AddItemToObject(object[0], "kills", object[1]);

        if (includeMeans)
        {
            cJSON_AddItemToObject(object[0], "kills_by_means", object[2]);
        }

        cJSON_AddItemToObject(*json, matchID, object[0]);
    }

    stream = cJSON_Print(*json);

    if (!stream)
    {
        UTILITIES_abort();
    }

    printf("%s", stream);

    free(stream);

    return ERR_NONE;
}

/**
 * @brief @ref QLP_report()
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
static int
report(ST_QLP *data)
{
    ST_KILL *kill;
    ST_MATCH *match;
    ST_REPORT report;
    cJSON *json;
    char *buffer;
    int i;
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
        UTILITIES_abort();
    }

    memset(report.matchReport, 0, sizeof(ST_MATCH_REPORT) * report.matchCount);

    for (i = 0; i < report.matchCount; i++)
    {
        report.matchReport[i].meanOfDeath = (ST_KILL_MEAN *) malloc(MEANS_OF_DEATH_LIST_SIZE);

        if (!report.matchReport[i].meanOfDeath)
        {
            UTILITIES_abort();
        }

        memcpy(report.matchReport[i].meanOfDeath, meansOfDeath, MEANS_OF_DEATH_LIST_SIZE);
    }

    json = NULL;

    i = 0;

    match = data->log.match;

    while (match)
    {
        buffer = match->buffer;

        while (buffer)
        {
            buffer = strstr(buffer, QLP_KEY_PLAYER);

            if (!buffer)
            {
                if (!report.matchReport[i].player) /* No players found */
                {
                    return ERR_INVALID_ARGUMENT;
                }

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

            appendPlayer(&(report.matchReport[i].player), buffer, (int) lenght);
        }

        kill = match->kill;

        while (kill)
        {
            if (!kill->buffer)
            {
                return ERR_INVALID_ARGUMENT;
            }

            buffer = kill->buffer;

            buffer = strstr(buffer + strlen(QLP_KEY_KILL), ":");

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

            if (strncmp(QLP_KEY_WORLD_PLAYER, buffer, lenght))
            {
                updatePlayer(report.matchReport[i].player, buffer, (int) lenght, 1);

                buffer = strstr(buffer, "by ");

                if (!buffer)
                {
                    return ERR_INVALID_ARGUMENT;
                }

                buffer += 3; /* "by " */

                updateMeanOfDeath(report.matchReport[i].meanOfDeath, buffer);
            }
            else
            {
                buffer = strstr(buffer, "killed ");

                if (!buffer)
                {
                    return ERR_INVALID_ARGUMENT;
                }

                buffer += strlen("killed ");

                lenght = (unsigned long) strstr(buffer, " by");

                if (!lenght)
                {
                    return ERR_INVALID_ARGUMENT;
                }

                lenght -= (unsigned long) buffer;

                updatePlayer(report.matchReport[i].player, buffer, (int) lenght, -1);
            }

            report.matchReport[i].killCount += 1;

            kill = kill->next;
        }

        i += 1;

        match = match->next;
    }

    retValue = createJSON(&report, &json);

    if (retValue)
    {
        return ERR_INVALID_ARGUMENT;
    }

    cJSON_Delete(json);

    return ERR_NONE;
}

/**
 * @brief Updates mean of death kill count.
 * 
 * @param list mean of death list
 * @param name mean of death name
 * 
 * @return int ERR_xxx
 */
static int
updateMeanOfDeath(ST_KILL_MEAN *list, char *name)
{
    int i;

    if (!list || !name)
    {
        return ERR_INVALID_ARGUMENT;
    }

    for (i = 0; i < TOTAL_MEANS_OF_DEATH; i++)
    {
        if (!memcmp(list[i].name, name, strlen(list[i].name)))
        {
            list[i].killCount++;

            return ERR_NONE;
        }
    }

    return ERR_INVALID_ARGUMENT;
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

    if (!strncmp(name, QLP_KEY_WORLD_PLAYER, lenght))
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
