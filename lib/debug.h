#ifndef LIB_DEBUG_H
#define LIB_DEBUG_H

#include <stdio.h>
#include <stdlib.h>


#ifdef NDEBUG
#define ASSERT(CONDITION) ((void) 0)
#else
#define ASSERT(CONDITION)                                     \
    if (!(CONDITION)) {                                       \
        printf("Assertion failed: %s, file %s, line %d\n",    \
               #CONDITION, __FILE__, __LINE__);               \
        abort();                                              \
    }
#endif

#endif 
