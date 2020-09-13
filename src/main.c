/**
 * @file main.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Entry point.
 * @date 2020-09-03
 * 
 */

#include "libqlp/qlp.h" /* (Q)uake (L)og (P)arser API */

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
    ST_QLP data;
    int retValue;

    retValue = QLP_start();

    if (retValue || argc <= 1)
    {
        terminate(argv[0], EXIT_FAILURE);
    }

    retValue = QLP_import(argv[1], &data);

    if (retValue)
    {
        terminate(argv[0], retValue);
    }

    retValue = QLP_evaluate(&data);

    if (retValue)
    {
        terminate(argv[0], retValue);
    }

    retValue = QLP_report(&data);

    if (retValue)
    {
        terminate(argv[0], retValue);
    }

    terminate(argv[0], EXIT_SUCCESS);

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
