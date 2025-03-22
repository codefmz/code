#ifndef __UTIL__H__
#define __UTIL__H__

#include <stdio.h>
#include <stdlib.h>

#define ASSERT_EQUAL(condition, var)                                     \
    do {                                                                 \
        if (condition != var) {                                          \
            fprintf(stderr, "Assertion failed: %s , file %s, line %d\n", \
                #condition, __FILE__, __LINE__);                         \
        }                                                                \
    } while (0)

#define ASSERT_NOT_EQUAL(condition, var)                                \
    do {                                                                \
        if (condition == var) {                                         \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", \
                #condition, __FILE__, __LINE__);                        \
        }                                                               \
    } while (0)

#define ASSERT_EQUAL_STR(condition, var)                                \
    do {                                                                \
        if (strcmp(condition, var) != 0) {                              \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", \
                #condition, __FILE__, __LINE__);                        \
        }                                                               \
    } while (0)

#endif