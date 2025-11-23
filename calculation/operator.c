#include <stdio.h>
#include <stdlib.h>

#include "array/_index.c"
#include "operant.c"
#include "expression.c"


void calcOperator(Expression *expression, VariableArray *variables, int operatorIndex)
{
    double result = 0;
    char operator = (expression->expDummy->str)[operatorIndex];

    Operant leftOp = {
        .operantStr = newCharArray(NULL, 0),
        .value = 0,
        .floatingPointSymbolIncluded = 0};
    Operant rightOp = {
        .operantStr = newCharArray(NULL, 0),
        .value = 0,
        .floatingPointSymbolIncluded = 0};

    // left
    int indexLeft = operatorIndex - 1;
    getLeftOperant(expression, variables, &leftOp, &indexLeft);

    // right
    int indexRight = operatorIndex + 1;
    getRightOperant(expression, variables, &rightOp, &indexRight);

    // calculate rusult
    switch (operator)
    {
    case '*':
        result = leftOp.value * rightOp.value;
        break;
    case '/':
        result = leftOp.value / rightOp.value;
        break;
    case '+':
        result = leftOp.value + rightOp.value;
        break;
    case '-':
        result = leftOp.value - rightOp.value;
        break;
    default:
        printf("\nSyntax Error:\n"
               "Invalid operator found.\n"
               "Invalid operator: %c\n",
               operator);
        exit(EXIT_FAILURE);
    }

    // clear evaluated characters; set their indexes to Space (32) ' '
    int clearStart = indexLeft + 1;
    int clearEnd = indexRight - 1;
    clearSpace(expression->expDummy, clearStart, clearEnd);

    incertDoubleInStrAtIndex(expression->expDummy, result, operatorIndex);
}