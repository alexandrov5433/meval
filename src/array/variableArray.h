#include <stdlib.h>

#ifndef VARAIBLE_ARRAY
#define VARAIBLE_ARRAY

/**
 * @param name The name of the Variable - a single character.
 * @param value The value of the Variable - a double.
 */
typedef struct Variable
{
    char name;
    double value;
} Variable;

/**
 * @param array An array of pointers to Variable-s.
 * @param length The length of the array property as an integer; the count of the Variable-s in array.
 */
typedef struct VariableArray
{
    struct Variable **array;
    int length;
} VariableArray;

Variable *newVariable(const char name, const double value);

VariableArray *newVariableArray();

void freeVariableArray(VariableArray *ptr);

void appendVariable(Variable *newVariable, VariableArray *variables);

Variable *findByName(const char name, const VariableArray *variables);

#endif