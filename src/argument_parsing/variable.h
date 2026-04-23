#ifndef ARGUMENT_PARSING_VARIABLE
#define ARGUMENT_PARSING_VARIABLE

#include <stdlib.h>
#include "galxlib/dynamic_array.h"

/**
 * @param name The name of the Variable - a single character.
 * @param value The value of the Variable - a double.
 */
typedef struct Variable
{
	char name;
	double value;
} Variable;

Variable *new_variable(const char name, const double value);

inline void free_variable(Variable *var);

int parse_variable(DynamicArray *var_arr, const int argc, char **argv, int *output_is_match);

#endif