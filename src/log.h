/**
 * @file log.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Namespace, class or interface definitions.
 * @date 2020-09-03
 * 
 */

#ifndef _LOG_H_INCLUDED_
#define _LOG_H_INCLUDED_

#include "parser.h"

/********************/
/* Type definitions */
/********************/

typedef enum
{
    LOG_ERR_DEFAULT = PAR_ERR_MARKER,
    LOG_ERR_ALREADY_STARTED,
    LOG_ERR_FILE_EMPTY,
    LOG_ERR_FILE_NOT_FOUND,
    LOG_ERR_INVALID_ARGUMENT,
    LOG_ERR_OUT_OF_MEMORY,
    LOG_ERR_MARKER,
    LOG_SUCCESS = 0
} EN_LOG_ERR;

/********************/
/* Public functions */
/********************/

/**
 * @brief Log file importer.
 *
 * @param[in] file file name
 * @param[out] log log file structure
 * 
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
extern int
LOG_import(const char *file, ST_PARSER *log);

/**
 * @brief Initialization function.
 *
 * @return int LOG_SUCCESS or LOG_ERR_xxx
 */
extern int
LOG_start(void);

#endif /* #ifndef _LOG_H_INCLUDED_ */
