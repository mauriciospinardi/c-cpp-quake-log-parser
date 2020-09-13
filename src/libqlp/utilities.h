/**
 * @file utilities.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Private namespace, class or interface definitions.
 * @date 2020-09-06
 * 
 */

#ifndef _UTILITIES_H_INCLUDED_
#define _UTILITIES_H_INCLUDED_

#include "qlp.h"

/**********/
/* Macros */
/**********/

#ifdef _DEBUG_
#define LIBQLP_TRACE(...) UTILITIES_log(__DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LIBQLP_TRACE(...) /* NULL */
#endif /* #ifdef _DEBUG_ */

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

/**
 * @brief Aborts execution with EXIT_FAILURE. For fatal errors only.
 * 
 */
extern void
UTILITIES_abort(void);

#endif /* #ifndef _UTILITIES_H_INCLUDED_ */
