#include <stdio.h>
#include <stdlib.h>

#include "array/_index.c"

void checkParenthesisSyntax(CharArray *expression)
{
    char *exp = expression->str;
    int expLength = expression->length;

    IntArray *markedIndexes = calloc(sizeof(IntArray), 1);
    markedIndexes->array = NULL; // memory added by realloc in appendInt()
    markedIndexes->length = calloc(sizeof(int), 1);
    markedIndexes->length = 0;

    // (((2 + 3)*5)-1)^2 + (4 - 1)

    /*
    from inside out
    cicles - find valid pair; mark indexes
    1 cicle - find unmarked parenthesis
    if unmarked exist -> syntax error, else OK
    O-komplexity ???????
    */

    int lastOpeningIndex = -1;

    for (int i = 0; i < expLength; i++)
    {
        char c = exp[i];
        if (c == '(')
        {
            if (includesInt(markedIndexes, i))
            {
                continue;
            }
            lastOpeningIndex = i;
        }
        else if (c == ')')
        {
            if (includesInt(markedIndexes, i))
            {
                continue;
            }
            if (lastOpeningIndex == -1)
            {
                // Syntax Error - closing parenthesis without opening one;
                printf("\nSyntax Error:\n"
                       "Found closing parenthesis without an opening one.\n"
                       "Index of character: %d\n",
                       i);
                exit(EXIT_FAILURE);
            }
            if (lastOpeningIndex == i - 1)
            {
                // case: empty parentheses ()
                // lastOpeningIndex is with 1 less than i (closing index). Eg. 2 and 3 (3 - 1 == 2)
                printf("\nSyntax Error:\n"
                       "Empty parentheses found '()'.\n"
                       "Index of opening parenthesis: %d\n",
                       "Index of closing parenthesis: %d\n",
                       lastOpeningIndex, i);
                exit(EXIT_FAILURE);
            }
            // first valid closing index
            appendInt(markedIndexes, lastOpeningIndex); // (
            appendInt(markedIndexes, i);                // )
            // reset loop
            lastOpeningIndex = -1;
            i = 0;
        }
    }

    for (int i = 0; i < expLength; i++)
    {
        if (exp[i] == '(' || exp[i] == ')')
        {
            if (includesInt(markedIndexes, i) == 0)
            {
                // Syntax Error
                printf("\nSyntax Error:\n"
                       "Found a solo parenthesis without a closing/opening one.\n"
                       "Index of character: %d\n",
                       i);
                exit(EXIT_FAILURE);
            }
        }
    }

    // clean-up
    if (markedIndexes->array != NULL)
    {
        free(markedIndexes->array);
    }
    free(markedIndexes);
}

/**
 * Increment the given integer (intToIncrement) without exceeding the given ceiling (ceiling).
 * Eg. if ceiling is 5 and the integer is 4, the integer will be incremented to 5.
 * If the integer is bigger than or equal to the ceiling (>=), nothing is done.
 */
void incrementWithMax(int *intToIncrement, int ceiling)
{
    if (ceiling > *intToIncrement)
    {
        *intToIncrement++;
    }
}