/**
 * @file qlp.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief (Q)uake (L)og (P)arser public namespace, class or interface
 * definitions.
 * @date 2020-09-03
 *
 */

#ifndef _QLP_H_INCLUDED_
#define _QLP_H_INCLUDED_

/********************/
/* Type definitions */
/********************/

typedef enum ERROR
{
    ERR_MARKER = -999,
    ERR_ALREADY_STARTED,
    ERR_FILE_EMPTY,
    ERR_FILE_NOT_FOUND,
    ERR_INVALID_ARGUMENT,
    ERR_OUT_OF_MEMORY,
    ERR_MATCH_NOT_FOUND,
    /* ERR_xxx */
    ERR_DEFAULT = -1, /* EXIT_FAILURE */
    ERR_NONE = 0 /* EXIT_SUCCESS */
} EN_ERROR;

typedef struct KILL
{
    char *buffer;
    struct KILL *next;
} ST_KILL;

typedef struct MATCH
{
    ST_KILL *kill;
    char *buffer;
    struct MATCH *next;
} ST_MATCH;

typedef struct LOG
{
    ST_MATCH *match;
    char *buffer;
    char *file;
} ST_LOG;

typedef struct QLP
{
    ST_LOG log;
} ST_QLP;

/********************/
/* Public functions */
/********************/

/**
 * @brief Evaluates the content of a previously imported log file.
 * 
 * @param[in,out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_evaluate(ST_QLP *data);

/**
 * @brief Imports a log file.
 *
 * @param[in] file file name
 * @param[out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_import(const char *file, ST_QLP *data);

/**
 * @brief Creates a JSON report of a previously evaluated log file.
 * 
 * @param[in] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
QLP_report(ST_QLP *data);

/**
 * @brief Initializes the QLP API.
 * 
 * @return int ERR_xxx
 */
extern int
QLP_start(void);

#endif /* #ifndef _QLP_H_INCLUDED_ */
