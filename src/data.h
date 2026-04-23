#ifndef DATA
#define DATA

#include <stdlib.h>
#include "calculation/_index.h"
#include "galxlib/dynamic_array.h"
#include "galxlib/gstring.h"

typedef struct Data
{
	Expression *expression;
	DynamicArray *variables;
} Data;

Data *new_data();

void free_data(Data *data);

int set_root_expression(String *exp, Data *data);

int evaluate(const Data *data);

#endif