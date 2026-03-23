#include <stdlib.h>

#ifndef CHAR_ARRAY
#define CHAR_ARRAY

/**
 * @param str The actual character array.
 * @param length The length of str.
 */
typedef struct CharArray
{
    char *str;
    size_t length;
} CharArray;

static void _incertString(char *strToIncertInto, const char *strToIncert, size_t strToIncertLength, size_t startIndex);

CharArray *newCharArray(const char *str, size_t length);

CharArray *newCharArrayFromDouble(double d);

void freeCharArray(CharArray *target);

void extendCharArray(CharArray *array, size_t n);

char getLastChar(CharArray *arr);

int getLastIndex(CharArray *arr);

void shiftOneRight(CharArray *arr, int startIndex);

void shiftRight(CharArray *arr, int startIndex, int shiftCount);

void appendCharTo(CharArray *arr, char c);

void appendStringTo(CharArray *arr, char *str, size_t strLength);

void prependCharTo(CharArray *arr, char c);

void replacePart(CharArray *arr, int startIndex, int endIndex, CharArray *replacement);

CharArray *copy(CharArray *arr, int startIndex, int endIndex);

void squish(CharArray *arr);

void incertDoubleInStrAtIndex(CharArray *exp, double d, int startIndex);

#endif