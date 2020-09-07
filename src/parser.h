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
 * @description Evaluates the content of a previously loaded log file.
 * 
 * @param[in,out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_evaluate(ST_PARSER *data);

/**
 * @description Imports a log file.
 *
 * @param[in] file file name
 * @param[out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_import(const char *file, ST_PARSER *data);

/**
 * @description Creates a JSON report of a previously evaluated log file.
 * 
 * @param[in] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_report(ST_PARSER *data);

/**
 * @description Initializes the PARSER API.
 * 
 * @return int ERR_xxx
 */
extern int
PARSER_start(void);

#endif /* #ifndef _PARSER_H_INCLUDED_ */
