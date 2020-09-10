/**
 * @file parser.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Namespace, class or interface definitions.
 * @date 2020-09-03
 * 
 */

#ifndef _PARSER_H_INCLUDED_
#define _PARSER_H_INCLUDED_

#include "log.h"

/********************/
/* Type definitions */
/********************/

typedef struct
{
    ST_LOG log;
} ST_PARSER;

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
PARSER_evaluate(ST_PARSER *data);

/**
 * @brief Imports a log file.
 *
 * @param[in] file file name
 * @param[out] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_import(const char *file, ST_PARSER *data);

/**
 * @brief Creates a JSON report of a previously evaluated log file.
 * 
 * @param[in] data parser file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_report(ST_PARSER *data);

/**
 * @brief Initializes the PARSER API.
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_start(void);

#endif /* #ifndef _PARSER_H_INCLUDED_ */
