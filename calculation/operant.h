#include <stdlib.h>

#include "expression.h"
#include "../array/_index.h"
#include "../regex/_index.h"

#ifndef CALCULATION_OPERANT
#define CALCULATION_OPERANT

typedef struct Operant
{
    CharArray *operantStr;
    double value;
    int floatingPointSymbolIncluded; // 1 if symbol ',' or '.' included, else 0 (default).
    int isExpression;                // 1 if expression, else 0 (default).
    int isVariable;                  // 1 if variable, else 0 (default).
    char operator;                   // the opearator on the left side of the operant in the expression.
} Operant;

Operant *newOperant();

void freeOperant(Operant *operant);

void appendCharToOperant(char c, Operant *op);

void prependCharToOperant(char c, Operant *op);

static int isOperantStringValid(Operant *op);

int extractPlaceholderInt(CharArray *placeholder);

void evaluateOperantValue(Expression *expression, VariableArray *variables, Operant *op);

void checkOperantEnd(Operant *op);

void checkFloatingPointIncluded(Operant *op);

void getLeftOperant(Expression *expression, VariableArray *variables, Operant *leftOp, int *indexLeft);

void getRightOperant(Expression *expression, VariableArray *variables, Operant *rightOp, int *indexRight);

#endif