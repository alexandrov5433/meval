#ifndef ARGUMENT_PARSING_EXPRESSION
#define ARGUMENT_PARSING_EXPRESSION

#include <stdlib.h>

int parse_expression(const int argc, char **argv, String **output_expression, int *output_is_match);

#endif