#include <stdio.h>
#include <stdlib.h>

#include "intArray.h"

/**
 * Creates a new IntArray.
 * @return The pointer to the new IntArray.
 */
IntArray *newIntArray() {
    IntArray *intArr = calloc(1, sizeof(IntArray));
    if (intArr == NULL) {
        printf("\nError: Could not allocate memory for new IntArray array.\n\n");
        exit(EXIT_FAILURE);
    }
    intArr->array = NULL;
    intArr->length = 0;
    return intArr;
}

/**
 * Frees the memory of an IntArray.
 * @param obj The pointer to the IntArray, which must be freed. If NULL, nothing is done.
 */
void freeIntArray(IntArray *obj) {
    if (obj == NULL) {
        return;
    }
    if (obj->array != NULL) {
        free(obj->array);
    }
    free(obj);
}

/**
 * Extends the integer array (array property) of the given IntArray (obj) with n slots.
 */
static void extendIntArray(IntArray *obj, size_t n) {
    int *temp = realloc(obj->array, sizeof(int) * (obj->length + n));
    if (temp == NULL) {
        printf("\nError: Could not reallocate IntArray array.\n\n");
        exit(EXIT_FAILURE);
    }
    obj->array = temp;
    obj->length += n;
}

/**
 * Searches for the given target interger in a given array of integers (arr).
 * The search is done linearly, from left to right.
 * @return
 * 1 if the the target is present in the array, 0 otherwise.
 */
int includesInt(IntArray *obj, int target) {
    for (int i = 0; i < obj->length; i++) {
        if ((obj->array)[i] == target) {
            return 1;
        }
    }
    return 0;
}

/**
 * Appends the element to the end of the array of IntArray.
 * The length of IntArray is increased by one.
 */
void appendInt(IntArray *obj, int element) {
    extendIntArray(obj, 1);
    (obj->array)[obj->length - 1] = element;
}

