#include <stdlib.h>

typedef struct Variable
{
    char name;
    double value;
} Variable;

typedef struct VariableArray
{
    Variable *array; //FIX: must be Variable **array
    int length;
} VariableArray;

/**
 * Appends the new Variable to the end of the VariableArray, extending the array with one.
 */
void append(Variable *newVariable, VariableArray *variables)
{
    Variable *temp = realloc(variables->array, sizeof(Variable) * (variables->length + 1));
    if (temp == NULL)
    {
        printf("\nError: Could not reallocate Variable array.\n\n");
        exit(EXIT_FAILURE);
    }
    variables->array = temp;
    // Because memory was increased by 1 slot by realloc, length is now the last index.
    (variables->array)[variables->length] = *newVariable;
    // Increment length to actualy be qual to the real length. 
    variables->length++;
}

/**
 * Finds the first Variable with this name in the Variable array.
 * The search is done from left to right.
 * @return The pointer to the first Variable with this name.
 * If no such Variable was found, a NULL pointer is returned.
 */
Variable *findByName(char name, VariableArray *variables)
{
    for (int i = 0; i < variables->length; i++)
    {
        if (variables->array[i].name == name)
        {
            return &(variables->array[i]);
        }
    }
    return NULL;
}