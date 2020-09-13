/**
 * @file qlp.h
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Private namespace, class or interface definitions.
 * @date 2020-09-03
 * 
 */

#ifndef _QLP_H_INCLUDED_

#include "libqlp/qlp.h" /* Public entry point */

#include "kill.h"
#include "log.h"
#include "match.h"
#include "utilities.h"

/**********/
/* Macros */
/**********/

#ifdef _UNIT_TESTS_
#define LIBQLP_PRINTF(...) /* NULL */
#else
#define LIBQLP_PRINTF(...) printf(__VA_ARGS__)
#endif /* _UNIT_TESTS_ */

#ifdef _DEBUG_
#define LIBQLP_TRACE(...) UTILITIES_log(__DATE__, __TIME__, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define LIBQLP_TRACE(...) /* NULL */
#endif /* #ifdef _DEBUG_ */

#define QLP_JSON_KEY_GAME "game_%d"
#define QLP_JSON_KEY_KILLS "kills"
#define QLP_JSON_KEY_KILLS_BY_MEANS "kills_by_means"
#define QLP_JSON_KEY_PLAYERS "players"
#define QLP_JSON_KEY_TOTAL_KILLS "total_kills"
#define QLP_KEY_KILL "Kill:"
#define QLP_KEY_MATCH "InitGame:"
#define QLP_KEY_PLAYER "ClientUserinfoChanged:"
#define QLP_KEY_WORLD_PLAYER "<world>"

#endif /* #ifndef _QLP_H_INCLUDED_ */
