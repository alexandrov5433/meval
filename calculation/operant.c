#include <stdlib.h>

#include "array/_index.c"

typedef struct Operant
{
    CharArray *operantStr;
    double value;
    int floatingPointSymbolIncluded; // can be ',' or '.'
} Operant;

/**
 * Extends the CharArray (operantStr) of the Operant by 1 and places the given character (c) at the end.
 */
void appendCharToOperant(char c, Operant *op)
{
    appendCharTo(op->operantStr, c);
}

/**
 * Extends the CharArray (operantStr) of the Operant by 1.
 * All elements are shifted to the right by 1 and the given character (c) is placed at the start (at index 0).
 * @return 0 on success, 1 otherwise.
 * 1 is returned when realloc fails (realloc returns NULL).
 */
int prependCharToOperant(char c, Operant *op)
{
    prependCharTo(op->operantStr, c);
}

void evaluateOperantValue(Operant *op)
{
    (op->value) = strtod((op->operantStr->str), NULL);
}

/**
 * If the Operant ends with '.' or ',', the program exits with EXIT_FAILURE.
 */
void checkOperantEnd(Operant *op)
{
    char lastChar = getLastChar(op->operantStr);

    if (lastChar == ',' || lastChar == '.')
    {
        printf("\nSyntax Error:\n",
               "A numerical operant can not end with '.' or ','.\n",
               "Current operant string: %s\n",
               op->operantStr->str);
        exit(EXIT_FAILURE);
    }
}

/**
 * If the Operant includes a floating point symbol ('.' or ','), the program exits with EXIT_FAILURE.
 * This check is done using the floatingPointSymbolIncluded property of Operant. 
 */
void checkFloatingPointIncluded(Operant *op)
{
    if (op->floatingPointSymbolIncluded)
    {
        printf("\nSyntax Error:\n"
               "A numerical operant may include only one floating point symbol ('.' or ',')!\n"
               "Failed while trying to add another floating point symbol ('.' or ',') to an operant, which already includes one.\n"
               "Current operant string: %s\n",
               op->operantStr->str);
        exit(EXIT_FAILURE);
    }
}

void getLeftOperant(CharArray *exp, Operant *leftOp, int *indexLeft)
{
    while (1)
    {
        // skip white space
        if (*indexLeft <= 0)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }

        char elem = (exp->str)[*indexLeft];

        if (elem != ' ')
        {
            break;
        }

        (*indexLeft)--;
    }

    while (1)
    {
        if (*indexLeft <= 0)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }
        char elem = (exp->str)[*indexLeft];
        // TODO: add mathematical constants, logarithm ect.

        if (elem >= 48 && elem <= 57)
        {
            // integer 0-9
            prependChar(elem, &leftOp);
        }
        else if (elem == '.' || elem == ',')
        {
            // floating point symbol
            checkFloatingPointIncluded(leftOp); // exit if included

            prependChar(elem, &leftOp);
            leftOp->floatingPointSymbolIncluded = 1;
        }
        else
        {
            checkOperantEnd(leftOp); // exit if invalid

            evaluateOperantValue(&leftOp);

            break;
        }
        (*indexLeft)--;
    }
}

void getRightOperant(CharArray *exp, Operant *rightOp, int *indexRight)
{
    char *e = exp->str;
    int expLength = exp->length;

    while (1)
    {
        // skip white space
        if (*indexRight >= expLength)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }

        char elem = e[*indexRight];

        if (elem != ' ')
        {
            break;
        }

        (*indexRight)++;
    }
    while (1)
    {
        if (*indexRight >= expLength)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }
        char elem = e[*indexRight];
        // TODO: add mathematical constants, logarithm ect.
        
        if (elem >= 48 && elem <= 57)
        {
            // integer 0-9
            appendChar(elem, &rightOp);
        }
        else if (elem == '.' || elem == ',')
        {
            // floating point symbol
            checkFloatingPointIncluded(rightOp); // exit if included

            appendChar(elem, &rightOp);
            rightOp->floatingPointSymbolIncluded = 1;
        }
        else
        {
            checkOperantEnd(rightOp); // exit if invalid

            evaluateOperantValue(&rightOp);

            break;
        }

        (*indexRight)++;
    }
}

