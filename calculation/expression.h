#include <stdlib.h>
#include "../array/_index.h"

#ifndef CALCULATION_EXPRESSION
#define CALCULATION_EXPRESSION

typedef struct Expression
{
    CharArray *exp;
    struct ExpressionArray *innerExpressions;
    struct Expression *parentExpression;
    double value;
} Expression;

typedef struct ExpressionArray
{
    struct Expression **array;
    size_t length;
} ExpressionArray;

ExpressionArray *deleteExpFromArray(Expression *exp, ExpressionArray *arr);

static void _freeExpression(Expression *ptr);

void freeExpression(Expression *ptr);

static void addInnerExpression(Expression *mainExp, Expression *innerExp);

static CharArray *createPlaceholder(int *n);

static void extractExpression(Expression *mainExp, int openIndex, int closeIndex, int *innerExpCounter);

static void parse(Expression *mainExp);

void calculateExpressionValue(Expression *expression, VariableArray *variables);

Expression *newExpression(CharArray *expression, Expression *parentExpression);

static ExpressionArray *newExpressionArray(Expression *exp);

#endif