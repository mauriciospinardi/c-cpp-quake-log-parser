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
    ST_PARSER data;
    int ret;

    ret = PARSER_start();

    if (ret)
    {
        terminate(argv[0], ERR_DEFAULT);
    }

    APPLICATION_TRACE("argc [%d] argv[0] [%s]", argc, argv[0]);

    if (argc <= 1)
    {
        terminate(argv[0], ERR_INVALID_ARGUMENT);
    }

    ret = PARSER_import(argv[1], &data);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    ret = PARSER_evaluate(&data);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    ret = PARSER_report(&data);

    if (ret)
    {
        terminate(argv[0], ret);
    }

    return EXIT_SUCCESS;
}

/*********************/
/* Private functions */
/*********************/

/**
 * @brief Show help and terminates execution.
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
