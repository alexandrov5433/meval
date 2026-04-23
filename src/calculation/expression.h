#ifndef CALCULATION_EXPRESSION
#define CALCULATION_EXPRESSION
#include <stdlib.h>
#include "galxlib/gstring.h"
#include "galxlib/dynamic_array.h"

typedef struct Expression
{
    String *exp;
    struct DynamicArray *inner_expressions;
    struct Expression *parent_expression;
    double value;
} Expression;
/* 
typedef struct ExpressionArray
{
    struct Expression **array;
    size_t length;
} ExpressionArray;
 */

int *new_expression(String *exp, Expression *parent_expression, Expression **output_exp);

ExpressionArray *deleteExpFromArray(Expression *exp, ExpressionArray *arr);

void free_expression(Expression *ptr);

void add_inner_expression(Expression *mainExp, Expression *innerExp);

CharArray *createPlaceholder(int *n);

void extractExpression(Expression *mainExp, int openIndex, int closeIndex, int *innerExpCounter);

void parse(Expression *mainExp);

void calculateExpressionValue(Expression *expression, VariableArray *variables);

#endif