#ifndef DATA
#define DATA

#include <stdlib.h>
#include "array/_index.h"
#include "calculation/_index.h"

typedef struct Data
{
    Expression *expression;
    VariableArray *variables;
} Data;

Data *newData();

void freeData(Data *data);

void setRootExpression(CharArray *exp, Data *data);

void evaluate(const Data *data);

#endif