#include <stdlib.h>

#include "array/_index.c"
#include "calculation/_index.c"
#include "regex/_index.c"

typedef struct Data
{
    Expression expression;
    VariableArray variables;
} Data;