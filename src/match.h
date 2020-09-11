/**
 * @file match.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Namespace, class or interface definitions.
 * @date 2020-09-06
 * 
 */

#ifndef _MATCH_H_INCLUDED_
#define _MATCH_H_INCLUDED_

#include "kill.h"

/********************/
/* Type definitions */
/********************/

typedef struct MATCH
{
    ST_KILL *kill;
    char *buffer;
    struct MATCH *next;
} ST_MATCH;

/********************/
/* Public functions */
/********************/

/**
 * @brief Evaluates the content of a previously imported match stream.
 * 
 * @param[in,out] data match file structure
 * 
 * @return int ERR_xxx
 */
extern int
MATCH_evaluate(ST_MATCH **data);

/**
 * @brief Imports a match stream.
 *
 * @param[in] stream match buffer
 * @param[in] size size of match stream
 * @param[out] data match stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
MATCH_import(const char *stream, unsigned long size, ST_MATCH **data);

/**
 * @brief Initializes the MATCH API.
 *
 * @return int ERR_NONE or ERR_xxx
 */
extern int
MATCH_start(void);

#endif /* #ifndef _MATCH_H_INCLUDED_ */
