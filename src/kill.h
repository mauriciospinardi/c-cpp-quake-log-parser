/**
 * @file kill.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Namespace, class or interface definitions.
 * @date 2020-09-10
 * 
 */

#ifndef _KILL_H_INCLUDED_
#define _KILL_H_INCLUDED_

#include "utilities.h"

/********************/
/* Type definitions */
/********************/

typedef struct KILL
{
    char *buffer;
    struct KILL *next;
} ST_KILL;

/********************/
/* Public functions */
/********************/

/**
 * @brief Imports a kill stream.
 *
 * @param[in] stream kill buffer
 * @param[in] size size of kill stream
 * @param[out] data kill stream structure
 * 
 * @return int ERR_NONE or ERR_xxx
 */
extern int
KILL_import(const char *stream, unsigned long size, ST_KILL **data);

/**
 * @brief Initializes the MATCH API.
 *
 * @return int ERR_NONE or ERR_xxx
 */
extern int
KILL_start(void);

#endif /* #ifndef _KILL_H_INCLUDED_ */
