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
    PARSER_TRACE("argc [%d] argc[0] [%s]", argc, argv[0]);

    if (argc <= 1)
    {
        terminate(argv[0], -1);
    }

    /* TODO: code */

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
    printf("\nUsage: %s [file]", executable);
    printf("\n");

    /* TODO: error translation */

    /* (ex.)
     * printf("\n    Error: [file] is missing");
     * printf("\n")
     */

    if (!error)
    {
        exit(EXIT_SUCCESS);
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}
