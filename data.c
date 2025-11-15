#include <stdlib.h>

#include "array/_index.c"
#include "calculation/_index.c"

typedef struct Data
{
    Expression expression;
    VariableArray variables;
} Data;

void calculateExpressionValue(Expression *expression, VariableArray *variables)
{
    if (expression->innerExpressions != NULL) {
        for (int i = 0; i < expression->innerExpressions->length; i++) {
            calculateExpressionValue((expression->innerExpressions->array)[i], variables);
        }
    }

    // 2 + #0#^2 + #1#*2
    // copy in order to make changes in the string without changing the original expression
    CharArray *expCopy = copy(expression->exp, 0, expression->exp->length - 1);
    char *expStr = expCopy->str;
    int expLength = expCopy->length;

    // operator: ^
    for (int exponentIndex = 0; exponentIndex < expLength; exponentIndex++)
    {
        char currentChar = expStr[exponentIndex];
        if (currentChar == '^')
        {
            calcOperator(expCopy, exponentIndex);
            exponentIndex = 0;
        }
    }
    // operator: *
    for (int multiplicationIndex = 0; multiplicationIndex < expLength; multiplicationIndex++)
    {
        char currentChar = expStr[multiplicationIndex];
        if (currentChar == '*')
        {
            calcOperator(expCopy, multiplicationIndex);
            multiplicationIndex = 0;
        }
    }
    // operator: /
    for (int divisionIndex = 0; divisionIndex < expLength; divisionIndex++)
    {
        char currentChar = expStr[divisionIndex];
        if (currentChar == '/')
        {
            calcOperator(expCopy, divisionIndex);
            divisionIndex = 0;
        }
    }
    // operator: +
    for (int additionIndex = 0; additionIndex < expLength; additionIndex++)
    {
        char currentChar = expStr[additionIndex];
        if (currentChar == '+')
        {
            calcOperator(expCopy, additionIndex);
            additionIndex = 0;
        }
    }
    // operator: -
    for (int subtractionIndex = 0; subtractionIndex < expLength; subtractionIndex++)
    {
        char currentChar = expStr[subtractionIndex];
        if (currentChar == '-')
        {
            calcOperator(expCopy, subtractionIndex);
            subtractionIndex = 0;
        }
    }

    // TODO: add logarithm, math. constants etc.

    // at this point, there should be only one number in the expression's string - the result
    expression->value = strtod(expStr, NULL);

    freeCharArray(expCopy);
}