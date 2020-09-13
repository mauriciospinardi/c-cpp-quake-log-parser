/**
 * @file tests.c
 * @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 * @platform cygwin64
 * @brief Unit tests entry point.
 * @date 2020-09-12
 * 
 */

#include "libqlp/qlp.h" /* (Q)uake (L)og (P)arser API */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************/
/* Type definitions */
/********************/

typedef struct UNIT_TESTS
{
    char name[6 + 1];
    int (*function)(void);
} ST_UNIT_TESTS;

/*********************/
/* Private functions */
/*********************/

/**
 * @brief @ref QLP_import() Invalid argument: file (NULL).
 * 
 * @return int ERR_xxx
 */
static int
UT0001(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(NULL, &data);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() Invalid argument: data (NULL).
 * 
 * @return int ERR_xxx
 */
static int
UT0002(void)
{
    int error;

    error = QLP_import(".\\tests\\UT0002.log", NULL);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() Invalid argument: file ('\0').
 * 
 * @return int ERR_xxx
 */
static int
UT0003(void)
{
    ST_QLP data;
    int error;

    error = QLP_import("", &data);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() File not found.
 * 
 * @return int ERR_xxx
 */
static int
UT0004(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\idontexist.youshouldnotfindme", &data);

    if (error != ERR_FILE_NOT_FOUND)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() Empty file.
 * 
 * @return int ERR_xxx
 */
static int
UT0005(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0005.log", &data);

    if (error != ERR_FILE_EMPTY)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() Wrong file content.
 * 
 * @return int ERR_xxx
 */
static int
UT0006(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0006.log", &data);

    switch (error)
    {
    case ERR_INVALID_ARGUMENT:
    case ERR_NONE: /* Acceptable */
        break;

    default:
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() Wrong file content w/ known key words.
 * 
 * @return int ERR_xxx
 */
static int
UT0007(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0007.log", &data);

    switch (error)
    {
    case ERR_INVALID_ARGUMENT:
    case ERR_NONE: /* Acceptable */
        break;

    default:
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_import() File successfully imported.
 * 
 * @return int ERR_xxx
 */
static int
UT0008(void)
{
    ST_QLP data;

    return QLP_import(".\\tests\\UT0008.log", &data);
}

/**
 * @brief @ref QLP_evaluate() Invalid argument: data (NULL).
 * 
 * @return int ERR_xxx
 */
static int
UT0009(void)
{
    int error;

    error = QLP_evaluate(NULL);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_evaluate() File not previously imported.
 * 
 * @return int ERR_xxx
 */
static int
UT0010(void)
{
    ST_QLP data;
    int error;

    memset(&data, 0, sizeof(ST_QLP));

    error = QLP_evaluate(&data);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_evaluate() Wrong file.
 * 
 * @return int ERR_xxx
 */
static int
UT0011(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0011.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    switch (error)
    {
    case ERR_DEFAULT:
    case ERR_INVALID_ARGUMENT:
    case ERR_MATCH_NOT_FOUND:
        break;

    default:
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_evaluate() Wrong file content w/ known key words.
 * 
 * @return int ERR_xxx
 */
static int
UT0012(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0012.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    switch (error)
    {
    case ERR_DEFAULT:
    case ERR_INVALID_ARGUMENT:
    case ERR_MATCH_NOT_FOUND:
        break;

    default:
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_evaluate() File successfully imported and evaluated.
 * 
 * @return int ERR_xxx
 */
static int
UT0013(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0013.log", &data);

    if (error)
    {
        return error;
    }

    return QLP_evaluate(&data);
}

/**
 * @brief @ref QLP_report() Invalid argument: data (NULL).
 * 
 * @return int ERR_xxx
 */
static int
UT0014(void)
{
    int error;

    error = QLP_report(NULL);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_report() File not previously imported.
 * 
 * @return int ERR_xxx
 */
static int
UT0015(void)
{
    ST_QLP data;
    int error;

    memset(&data, 0, sizeof(ST_QLP));

    error = QLP_report(&data);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_report() File not previously evaluated.
 * 
 * @return int ERR_xxx
 */
static int
UT0016(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0016.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_report(&data);

    QLP_free(&data);

    if (error != ERR_INVALID_ARGUMENT)
    {
        return error;
    }

    return ERR_NONE;
}

/**
 * @brief @ref QLP_report() File successfully imported and evaluated. Report
 * generated. Single match. No kills. Tiny file (~10 lines).
 *
 * @return int ERR_xxx
 */
static int
UT0017(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0017.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    if (error)
    {
        return error;
    }

    return QLP_report(&data);
}

/**
 * @brief @ref QLP_report() File successfully imported and evaluated. Report
 * generated. Single match. Small file (~100 lines).
 *
 * @return int ERR_xxx
 */
static int
UT0018(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0018.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    if (error)
    {
        return error;
    }

    return QLP_report(&data);
}

/**
 * @brief @ref QLP_report() File successfully imported and evaluated. Report
 * generated. Single match. Medium file (~500 lines).
 *
 * @return int ERR_xxx
 */
static int
UT0019(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0019.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    if (error)
    {
        return error;
    }

    return QLP_report(&data);
}

/**
 * @brief @ref QLP_report() File successfully imported and evaluated. Report
 * generated. Multi-match. Big file (~5000 lines).
 *
 * @return int ERR_xxx
 */
static int
UT0020(void)
{
    ST_QLP data;
    int error;

    error = QLP_import(".\\tests\\UT0020.log", &data);

    if (error)
    {
        return error;
    }

    error = QLP_evaluate(&data);

    if (error)
    {
        return error;
    }

    return QLP_report(&data);
}

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
    int error;
    int i;

    static ST_UNIT_TESTS ut[] =
    {
         { "UT0001", UT0001 }
        ,{ "UT0002", UT0002 }
        ,{ "UT0003", UT0003 }
        ,{ "UT0004", UT0004 }
        ,{ "UT0005", UT0005 }
        ,{ "UT0006", UT0006 }
        ,{ "UT0007", UT0007 }
        ,{ "UT0008", UT0008 }
        ,{ "UT0009", UT0009 }
        ,{ "UT0010", UT0010 }
        ,{ "UT0011", UT0011 }
        ,{ "UT0012", UT0012 }
        ,{ "UT0013", UT0013 }
        ,{ "UT0014", UT0014 }
        ,{ "UT0015", UT0015 }
        ,{ "UT0016", UT0016 }
        ,{ "UT0017", UT0017 }
        ,{ "UT0018", UT0018 }
        ,{ "UT0019", UT0019 }
        ,{ "UT0020", UT0020 }
    };

    if (argc != 1)
    {
        printf("\nUsage: %s", argv[0]);
        printf("\n    Error: unexpected argument(s)");
        printf("\n");

        return EXIT_FAILURE;
    }

    if (QLP_start())
    {
        printf("\nUsage: %s", argv[0]);
        printf("\n    Error: unable to run QLP_start() succesfully");
        printf("\n");

        return EXIT_FAILURE;
    }

    printf("\nRunning unit tests...");
    printf("\n");

    for (i = 0; i < (int) (sizeof(ut) / sizeof(ST_UNIT_TESTS)); i++)
    {
        printf("\n    %s...", ut[i].name);

        error = ut[i].function();

        if (error)
        {
            printf(" %d FAILED\n\n", error);

            exit(EXIT_SUCCESS); /* UT FAILED, but tester ran OK */
        }

        printf(" OK");
    }

    printf("\n");

    return EXIT_SUCCESS;
}
