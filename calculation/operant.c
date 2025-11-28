#include <stdlib.h>
#include <stdio.h>

#include "operant.h"
#include "../array/_index.h"
#include "expression.h"
#include "../regex/_index.h"

Operant *newOperant()
{
    Operant *operant = calloc(1, sizeof(Operant));
    if (operant == NULL)
    {
        printf("\nError: Could not allocate memory for a new Operant.\n\n");
        exit(EXIT_FAILURE);
    }
    operant->operantStr = newCharArray(NULL, 0);
    operant->value = 0.0;
    operant->floatingPointSymbolIncluded = 0;
    operant->isExpression = 0;
    operant->isVariable = 0;
    return operant;
}

/**
 * Frees the memory allocated for the Operant.
 * @param operant A pointer to the Operant, which must be freed.
 */
void freeOperant(Operant *operant)
{
    freeCharArray(operant->operantStr);
    free(operant);
}

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
void prependCharToOperant(char c, Operant *op)
{
    prependCharTo(op->operantStr, c);
}

int extractPlaceholderInt(CharArray *placeholder)
{
    // #2134#
    RegexContainer *container = newRegexContainer(REGEX_EXP_PLACEHOLDER, 2, REG_EXTENDED);
    match(placeholder->str, container);
    char *str = getGroupValue(1, container);
    if (str == NULL)
    {
        printf("\nError: Could not extract placeholder integer.\n\n");
        exit(EXIT_FAILURE);
    }

    int result = atoi(str);
    free(str);
    return result;
}

void evaluateOperantValue(Expression *expression, VariableArray *variables, Operant *op)
{
    /*
        1. inner expression - #123#
        2. variable - x, y, z ...
        3. double or integer - 1209347
    */
    if (op->isExpression == 1)
    {
        int i = extractPlaceholderInt(op->operantStr);
        op->value = ((expression->innerExpressions->array)[i])->value;
    }
    else if (op->isVariable == 1)
    {
        char varName = (op->operantStr->str)[0];
        Variable *var = findByName(varName, variables);
        if (var == NULL)
        {
            printf("\nError: Could not find Variable with the '%c' character.\n\n", varName);
            exit(EXIT_FAILURE);
        }
        op->value = var->value;
    }
    else
    {
        op->value = strtod(op->operantStr->str, NULL);
    }
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

void getLeftOperant(Expression *expression, VariableArray *variables, Operant *leftOp, int *indexLeft)
{
    char *exp = expression->expDummy->str;
    // skip white space
    while (1)
    {
        if (*indexLeft <= 0)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }

        char elem = exp[*indexLeft];

        if (elem != ' ')
        {
            break;
        }

        (*indexLeft)--;
    }

    // case: inner expression placeholder
    if (exp[*indexLeft] == '#')
    {
        leftOp->isExpression = 1;
        leftOp->isVariable = 0;
        prependCharToOperant('#', leftOp); // first #
        (*indexLeft)--;

        while (1)
        {
            if (*indexLeft <= 0)
            {
                printf("\nSyntax Error:\n"
                       "The expression ended unexpectedly, while evaluating an operant.\n");
                exit(EXIT_FAILURE);
            }
            char elem = exp[*indexLeft];
            if (elem >= 48 && elem <= 57)
            {
                // integer 0-9
                prependCharToOperant(elem, leftOp);
            }
            else if (elem == '#')
            {
                // last #
                prependCharToOperant(elem, leftOp);
                evaluateOperantValue(expression, variables, leftOp);
                return;
            }
            (*indexLeft)--;
        }
    }
    else if (exp[*indexLeft] > 96 && exp[*indexLeft] < 123)
    // case: variable
    {
        // ASCII 'a' == 97 ; 'z' == 122
        leftOp->isExpression = 0;
        leftOp->isVariable = 1;
        appendCharToOperant(exp[*indexLeft], leftOp);
        evaluateOperantValue(expression, variables, leftOp);
        return;
    }

    // case: integer or double
    leftOp->isExpression = 0;
    leftOp->isVariable = 0;
    while (1)
    {
        if (*indexLeft <= 0)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }
        char elem = exp[*indexLeft];
        // TODO: add mathematical constants, logarithm ect.

        if (elem >= 48 && elem <= 57)
        {
            // integer 0-9
            prependCharToOperant(elem, leftOp);
        }
        else if (elem == '.' || elem == ',')
        {
            // floating point symbol
            checkFloatingPointIncluded(leftOp); // exit if included

            prependCharToOperant(elem, leftOp);
            leftOp->floatingPointSymbolIncluded = 1;
        }
        else
        {
            checkOperantEnd(leftOp); // exit if invalid

            evaluateOperantValue(expression, variables, leftOp);

            break;
        }
        (*indexLeft)--;
    }
}

void getRightOperant(Expression *expression, VariableArray *variables, Operant *rightOp, int *indexRight)
{
    char *exp = expression->expDummy->str;
    int expLength = expression->expDummy->length;

    // skip white space
    while (1)
    {
        if (*indexRight >= expLength)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }

        char elem = exp[*indexRight];

        if (elem != ' ')
        {
            break;
        }

        (*indexRight)++;
    }

    // case: inner expression placeholder
    if (exp[*indexRight] == '#')
    {
        rightOp->isExpression = 1;
        rightOp->isVariable = 0;
        appendCharToOperant('#', rightOp); // first #
        (*indexRight)++;

        while (1)
        {
            if (*indexRight >= expLength)
            {
                printf("\nSyntax Error:\n"
                       "The expression ended unexpectedly, while evaluating an operant.\n");
                exit(EXIT_FAILURE);
            }
            char elem = exp[*indexRight];
            if (elem >= 48 && elem <= 57)
            {
                // integer 0-9
                appendCharToOperant(elem, rightOp);
            }
            else if (elem == '#')
            {
                // last #
                appendCharToOperant(elem, rightOp);
                evaluateOperantValue(expression, variables, rightOp);
                return;
            }
            (*indexRight)++;
        }
    }
    else if (exp[*indexRight] > 96 && exp[*indexRight] < 123)
    {
        // case: variable
        // 'a' == 97 ; 'z' == 122
        rightOp->isExpression = 0;
        rightOp->isVariable = 1;
        appendCharToOperant(exp[*indexRight], rightOp); // first #
        evaluateOperantValue(expression, variables, rightOp);
        return;
    }

    // case: intiger or double
    rightOp->isExpression = 0;
    rightOp->isVariable = 0;
    while (1)
    {
        if (*indexRight >= expLength)
        {
            printf("\nSyntax Error:\n"
                   "The expression ended unexpectedly, while evaluating an operant.\n");
            exit(EXIT_FAILURE);
        }
        char elem = exp[*indexRight];
        // TODO: add mathematical constants, logarithm ect.

        if (elem >= 48 && elem <= 57)
        {
            // integer 0-9
            appendCharToOperant(elem, rightOp);
        }
        else if (elem == '.' || elem == ',')
        {
            // floating point symbol
            checkFloatingPointIncluded(rightOp); // exit if included

            appendCharToOperant(elem, rightOp);
            rightOp->floatingPointSymbolIncluded = 1;
        }
        else
        {
            checkOperantEnd(rightOp); // exit if invalid

            evaluateOperantValue(expression, variables, rightOp);

            break;
        }
        (*indexRight)++;
    }
}
