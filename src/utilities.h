/**
 * @file utilities.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Utilities definitions.
 * @date 2020-09-06
 * 
 */

#ifndef _UTILITIES_H_INCLUDED_
#define _UTILITIES_H_INCLUDED_

/**********/
/* Macros */
/**********/

#ifdef _DEBUG_
#define APPLICATION_TRACE(...) UTILITIES_log(__DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define APPLICATION_TRACE(...) /* NULL */
#endif /* #ifdef _DEBUG_ */

#define PARSER_KEY_KILL "Kill:"
#define PARSER_KEY_MATCH "InitGame:"
#define PARSER_KEY_PLAYER "ClientUserinfoChanged:"
#define PARSER_KEY_WORLD "<world>"

/********************/
/* Type definitions */
/********************/

typedef enum
{
    ERR_DEFAULT = -999,
    ERR_ALREADY_STARTED,
    ERR_FILE_EMPTY,
    ERR_FILE_NOT_FOUND,
    ERR_INVALID_ARGUMENT,
    ERR_OUT_OF_MEMORY,
    ERR_MATCH_NOT_FOUND,
    /* ERR_MARKER */
    ERR_NONE = 0
} EN_ERROR;

/********************/
/* Public functions */
/********************/

/**
 * @brief Log channel.
 * 
 * @param[in] format autodescriptive
 * @param[in] ... variable number of arguments
 */
extern void
UTILITIES_log(const char *date, const char *time, const char *file, const int line, const char *function, const char *format, ...);

/**
 * @brief Initializes the UTILITIES API.
 * 
 * @return int ERR_xxx
 */
extern int
UTILITIES_start(void);

#endif /* #ifndef _UTILITIES_H_INCLUDED_ */
