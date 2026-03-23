#include <stdlib.h>
#include <stdio.h>

#include "operant.h"
#include "../array/_index.h"
#include "expression.h"
#include "../regex/_index.h"

/**
 * Creates a new Operant.
 * @return The pointer to the new Operant.
 */
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
    operant->operator = '\0';
    return operant;
}

Operant *newOperantFromDouble(double d) {
    Operant *operant = calloc(1, sizeof(Operant));
    if (operant == NULL)
    {
        printf("\nError: Could not allocate memory for a new Operant.\n\n");
        exit(EXIT_FAILURE);
    }
    operant->operantStr = newCharArrayFromDouble(d);
    operant->value = d;
    operant->floatingPointSymbolIncluded = 1;
    operant->isExpression = 0;
    operant->isVariable = 0;
    operant->operator = d >= 0 ? '+' : '-';
    return operant;
}

/**
 * Frees the memory allocated for the Operant.
 * @param operant A pointer to the Operant, which must be freed.
 */
void freeOperant(Operant *operant)
{
    if (operant == NULL)
    {
        return;
    }
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
 */
void prependCharToOperant(char c, Operant *op)
{
    prependCharTo(op->operantStr, c);
}

static int isOperantStringValid(Operant *op)
{
    if (op->operantStr->length > 0)
    {
        return 1;
    }
    return 0;
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

    if (op->operator == '-' && op->value > 0)
    {
        op->value *= -1.0;
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