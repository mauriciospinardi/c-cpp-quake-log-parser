/**
 * @file main.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Entry point.
 * @date 2020-09-03
 * 
 */

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/***********************/
/* Function prototypes */
/***********************/

static void
terminate(const char *executable, int error);

/********************/
/* Public functions */
/********************/

/**
 * @brief Entry point.
 * 
 * @param[in] argc argument count
 * @param[in] argv argument vector
 * 
 * @return int EXIT_xxx
 * 
 */
int main(int argc, char *argv[])
{
    ST_PARSER log;
    int ret;

    PARSER_TRACE("argc [%d] argv[0] [%s]", argc, argv[0]);

    ret = PARSER_start();

    if (ret)
    {
        terminate(argv[0], PAR_ERR_DEFAULT);
    }

    if (argc <= 1)
    {
        terminate(argv[0], LOG_ERR_INVALID_ARGUMENT);
    }

    ret = PARSER_import(argv[1], &log);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    ret = PARSER_evaluate(&log);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    ret = PARSER_report(&log);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    terminate(argv[0], 0);

    return EXIT_FAILURE; /* Should never hit this line! */
}

/*********************/
/* Private functions */
/*********************/

/**
 * @description Show help and terminates execution.
 * 
 * @param[in] executable current executable path (can be relative)
 * @param[in] error autodescriptive
 */
static void
terminate(const char *executable, int error)
{
    if (!error)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("\nUsage: %s [file]", executable);
        printf("\n    Error: %d", error);
        printf("\n");

        /* TODO: error translation */

        /* (ex.)
        * printf("\n    Error: [file] is missing");
        * printf("\n")
        */

        exit(EXIT_FAILURE);
    }
}
