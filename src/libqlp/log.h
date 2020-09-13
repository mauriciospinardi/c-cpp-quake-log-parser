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

#include "qlp.h"

/********************/
/* Public functions */
/********************/

/**
 * @brief Evaluates the content of a previously imported log file.
 * 
 * @param[in,out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
LOG_evaluate(ST_LOG *data);

/**
 * @brief Imports a log file.
 *
 * @param[in] file file name
 * @param[out] data log file structure
 * 
 * @return int ERR_xxx
 */
extern int
LOG_import(const char *file, ST_LOG *data);

/**
 * @brief Initializes the LOG API.
 *
 * @return int ERR_xxx
 */
extern int
LOG_start(void);

#endif /* #ifndef _LOG_H_INCLUDED_ */
