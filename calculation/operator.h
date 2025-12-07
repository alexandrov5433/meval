#include <stdlib.h>

#include "../array/_index.h"
#include "expression.h"
#include "../regex/_index.h"

#ifndef CALCULATION_OPERATOR
#define CALCULATION_OPERATOR

void calcOperator(Expression *expression, VariableArray *variables, int operatorIndex);

#endif