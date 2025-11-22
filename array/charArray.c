#include <stdio.h>
#include <stdlib.h>

/**
 * @param str The actual character array.
 * @param length The length of str.
 */
typedef struct CharArray
{
    char *str;
    size_t length;
} CharArray;

static void _incertString(char *strToIncertInto, const char *strToIncert, size_t strToIncertLength, size_t startIndex)
{
    for (size_t i = 0; i < strToIncertLength; i++)
    {
        strToIncertInto[startIndex + i] = strToIncert[i];
    }
}

/**
 * Creates a new CharArray and returns it's pointer.
 * @param str A pointer to a string to copy into the new CharArray.
 * This pointer can be freed afterwords, bacause the contents of the char *str are copied over to the new CharArray.
 * If this argument is NULL - no copying is done.
 * @param length The length of the string contained in the char *str pointer of the str argument.
 *
 * If str is NULL and length is 0 - a new CharArray with .str = NULL and .length = 0 is created.
 * If str is NULL and length is bigger than 0 - a new CharArray,
 * with .str pointer to a char[] with the given length, with all elements initialized as '\0' (ASCII 0),
 * and .length equal to the given length, is created.
 *
 * If length is less than 0, the program exits with status code EXIT_FAILURE.
 */
CharArray *newCharArray(const char *str, size_t length)
{
    if (length < 0)
    {
        printf("\nError: Can not initialize a new CharArray with length 0.\n\n");
        exit(EXIT_FAILURE);
    }

    CharArray *newArr = calloc(1, sizeof(CharArray));
    if (newArr == NULL)
    {
        printf("\nError: Could not allocate memory for a new CharArray.\n\n");
        exit(EXIT_FAILURE);
    }

    char *newStr;
    if (length == 0)
    {
        newStr = NULL;
    }
    else
    {
        newStr = calloc(length, sizeof(char));
    }

    newArr->str = newStr;
    newArr->length = length;

    if (newStr != NULL && str != NULL)
    {
        _incertString(newArr->str, str, length, 0);
    }

    return newArr;
}

/**
 * Frees the memory allocated for the CharArray.
 * If the inner char *str property of the CharArray is NULL, it is skipped. Otherwise, it is also freed.
 * @param target Pointer to the CharArray, which is to be freed.
 */
void freeCharArray(CharArray *target)
{
    if (target->str != NULL)
    {
        free(target->str);
    }
    free(target);
}



/**
 * Extends a character array (array) by a given amount (n) of slots.
 */
void extendCharArray(CharArray *array, size_t n)
{
    char *temp = realloc(array->str, sizeof(char) * (array->length + n));
    if (temp == NULL)
    {
        printf("\nError: Could not reallocate CharArray array.\n\n");
        exit(EXIT_FAILURE);
    }
    array->str = temp;
    array->length += n;
}

/**
 * Convertes the a range of characters of a string to Space.
 * The range for conversion is from (including) startIndex to (including) endIndex.
 * Space is the character represented by ' ', with an ASCII value of 32.
 */
void clearSpace(CharArray *arr, int startIndex, int endIndex)
{
    // TODO: remove function ???
    for (int i = startIndex; i <= endIndex; i++)
    {
        (arr->str)[i] = ' ';
    }
}

/**
 * Returns the last element of the CharArray (length - 1).
 */
char getLastChar(CharArray *arr)
{
    if (arr->length == 0)
    {
        printf("\nError: Can not get the last character of a CharArray with length 0.\n"
               "The (char *str)-property is NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    return (arr->str)[arr->length - 1];
}

/**
 * Returns the number of the last index of the CharArray (length - 1).
 */
int getLastIndex(CharArray *arr)
{
    if (arr == NULL)
    {
        printf("\nError: A CharArray initilized with a (char *str)-property as NULL, does not have a last index.\n\n");
        exit(EXIT_FAILURE);
    }
    return (arr->length - 1);
}

/**
 * Moves all elements of the CharArray once to the right, starting from startIndex.
 * This function does not check array boundaries!
 */
void shiftOneRight(CharArray *arr, int startIndex)
{
    for (int i = arr->length - 1; i > startIndex; i--)
    {
        (arr->str)[i] = (arr->str)[i - 1];
    }
}

void shiftRight(CharArray *arr, int startIndex, int shiftCount)
{
    while (shiftCount > 0)
    {
        shiftOneRight(arr, startIndex);
        shiftCount--;
    }
}

void appendCharTo(CharArray *arr, char c)
{
    extendCharArray(arr, 1);
    (arr->str)[arr->length - 1] = c;
}

void appendStringTo(CharArray *arr, char *str, size_t strLength)
{
    if (strLength == 0)
        return; // or exit with error ???
    int formerLastIndex = getLastIndex(arr);
    extendCharArray(arr, strLength);
    int startIndex = formerLastIndex + 1;

    for (size_t i = 0; i < strLength - 1; i++)
    {
        (arr->str)[startIndex + i] = str[i];
    }
}

void prependCharTo(CharArray *arr, char c)
{
    extendCharArray(arr, 1);
    shiftOneRight(arr, 0);
    (arr->str)[0] = c;
}

void replacePart(CharArray *arr, int startIndex, int endIndex, CharArray *replacement)
{

    int replaceCount = endIndex - startIndex + 1;
    size_t newStrLength = (arr->length - replaceCount) + replacement->length;
    char *newStr = calloc(sizeof(char), newStrLength);

    for (int i = 0; i < startIndex; i++)
    {
        newStr[i] = (arr->str)[i];
    }
    for (int i = 0; i < replacement->length; i++)
    {
        newStr[startIndex + i] = (replacement->str)[i];
    }
    for (int i = 0; i < newStrLength; i++)
    {
        newStr[startIndex + replacement->length + i] = (arr->str)[endIndex + 1 + i];
    }

    free(arr->str);
    arr->str = newStr;
    arr->length = newStrLength;
}

/**
 * Copies a part of a CharArray and returns it as a new CharArray.
 */
CharArray *copy(CharArray *arr, int startIndex, int endIndex)
{
    if (startIndex > endIndex)
    {
        printf("\nError: Invalid function arguments for CharArray *copy().\n"
               "StartIndex can not be larger than endIndex.\n\n");
        exit(EXIT_FAILURE);
    }
    if (endIndex > arr->length || endIndex == arr->length) {
        printf("\nError: Invalid function arguments for CharArray *copy().\n"
               "EndIndex must be less than the length of the given CharArray.\n\n");
        exit(EXIT_FAILURE);
    }
    int copyLength = (endIndex - startIndex) + 1;

    CharArray *copy = newCharArray(NULL, copyLength);

    for (int i = 0; i < copyLength; i++)
    {
        (copy->str)[i] = (arr->str)[startIndex + i];
    }

    return copy;
}

/**
 * Removes all white space (ASCII 32) from the CharArray.
 * @param arr The CharArray from which the characters will be removed.
 * If the CharArray has a char *str property equal to NULL, nothing is done.
 */
void squish(CharArray *arr)
{
    if (arr->str == NULL)
    {
        return;
    }

    int spaceNeeded = 0;
    for (int i = 0; i < arr->length; i++)
    {
        char c = (arr->str)[i];
        if (c != 0 && c != 32)
        {
            spaceNeeded++;
        }
    }
    char *squishedStr = calloc(sizeof(char), spaceNeeded);
    if (squishedStr == NULL)
    {
        printf("\nError: Could not allocate memory for a new char *str.\n\n");
        exit(EXIT_FAILURE);
    }

    int currentIndexInSquished = 0;

    for (int i = 0; i < arr->length; i++)
    {
        char c = (arr->str)[i];
        if (c != 0 && c != 32)
        {
            squishedStr[currentIndexInSquished] = c;
            currentIndexInSquished++;
        }
    }

    free(arr->str);
    arr->str = squishedStr;
    arr->length = spaceNeeded;
}

/**
 * Converts the double to string and incerts the resulting string into the targeted string.
 * The incertion starts from startIndex and continues to the right. Only startIndex is replaced.
 * The first character of the converted double will be found on startIndex.
 * The targeted string will be expaned by the number of characters in the double's string.
 */
void incertDoubleInStrAtIndex(CharArray *exp, double d, int startIndex)
{
    // printf("%.6f\n", __DBL_MAX__);
    // prints a number with 316 characters
    // 4 extra char spaces added to prevent undefined behaviour of sprintf
    char *buffer = calloc(320, sizeof(char));
    sprintf(buffer, "%.6f", d);
    int doubleLength = 0;
    for (int i = 319; i > 0; i--)
    {
        if (buffer[i] != 0)
        {
            doubleLength = i + 1;
            break;
        }
    }

    extendCharArray(exp, doubleLength);
    shiftRight(exp, startIndex, doubleLength);

    for (int i = 0; i < doubleLength; i++)
    {
        (exp->str)[startIndex + i] = buffer[i];
    }

    free(buffer);
}