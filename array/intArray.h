#include <stdlib.h>

#ifndef INT_ARRAY
#define INT_ARRAY

typedef struct IntArray {
    int *array;
    size_t length;
} IntArray;

IntArray *newIntArray();

void freeIntArray(IntArray *obj);

static void extendIntArray(IntArray *obj, size_t n);

int includesInt(IntArray *obj, int target);

void appendInt(IntArray *obj, int element);

#endif