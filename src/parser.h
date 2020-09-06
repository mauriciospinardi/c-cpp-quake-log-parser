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

/**********/
/* Macros */
/**********/

#define API_PRIVATE __attribute__((visibility("hidden")))

#define API_PROTECTED __attribute__((visibility("protected")))

#define API_PUBLIC __attribute__((visibility("default")))

#ifdef _DEBUG_
#define PARSER_TRACE(...) PARSER_log(__DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define PARSER_TRACE(...) /* NULL */
#endif /* #ifdef _DEBUG_ */

/********************/
/* Type definitions */
/********************/

typedef enum
{
    PAR_ERR_DEFAULT = -999,
    PAR_ERR_INVALID_ARGUMENT,
    PAR_ERR_OUT_OF_MEMORY,
    PAR_ERR_ALREADY_STARTED,
    /* ... */
    PAR_ERR_NEXT,
    PAR_SUCCESS = 0
} EN_PAR_ERR;

/********************/
/* Public functions */
/********************/

/**
 * @description Log channel.
 * 
 * @param[in] format autodescriptive
 * @param[in] ... variable number of arguments
 */
extern void
PARSER_log(const char *date, const char *time, const char *file, const int line, const char *function, const char *format, ...);

/**
 * @brief Initialization function.
 * 
 * @return int PARSER_SUCCESS or PARSER_ERR_xxx
 */
extern int
PARSER_start(void);

/***********************/
/* Public dependencies */
/***********************/

#include "log.h"

#endif /* #ifndef _PARSER_H_INCLUDED_ */
