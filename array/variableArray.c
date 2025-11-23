#include <stdlib.h>

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
    Variable **array;
    int length;
} VariableArray;

/**
 * Creates a new VariableArray.
 * @return The pointer to the new VaribleArray.
 */
VariableArray *newVariableArray()
{
    VariableArray *varArr = calloc(1, sizeof(VariableArray));
    if (varArr == NULL)
    {
        printf("\nError: Could not allocate memory for a new VariableArray.\n\n");
        exit(EXIT_FAILURE);
    }

    varArr->array = NULL;
    varArr->length = 0;

    return varArr;
}

/**
 * Frees the memory used by a VariableArray.
 * @param ptr The pointer to the VariableArray, which must be freed. If NULL, nothing is done.
 * 
 * All Variable-s contained in the VariableArray are also freed, if there are any.
 */
void freeVariableArray(VariableArray *ptr) {
    if (ptr == NULL) {
        return;
    }
    if (ptr->array != NULL) {
        for (int i = 0; i < ptr->length; i++) {
            free((ptr->array)[i]);
        }
    }
    free(ptr);
}

/**
 * Appends the pointer of a Variable to the end of the VariableArray, extending the array with one.
 * @param newVariable The pointer to the Variable, which must be appended to the VariableArray.
 * @param variables The pointer to the VariableArray, at the end of which the pointer to the Variable will be appended.
 */
void append(const Variable *newVariable, VariableArray *variables)
{
    Variable **temp = realloc(variables->array, sizeof(Variable) * (variables->length + 1));
    if (temp == NULL)
    {
        printf("\nError: Could not reallocate Variable array.\n\n");
        exit(EXIT_FAILURE);
    }
    variables->array = temp;
    // Because memory was increased by 1 slot by realloc, length is now the last index.
    (variables->array)[variables->length] = newVariable;
    // Increment length to actualy be qual to the real length.
    variables->length++;
}

/**
 * Finds the first Variable with this name in the Variable array.
 * The search is done from left to right.
 * @param name The character, which is the name of the Variable.
 * @param variables The pointer to the VariableArray, which should contain the Variable.
 * @return The pointer to the first Variable with this name.
 * If no such Variable was found, a NULL pointer is returned.
 */
Variable *findByName(const char name, const VariableArray *variables)
{
    for (int i = 0; i < variables->length; i++)
    {
        if ((variables->array)[i]->name == name)
        {
            return (variables->array)[i];
        }
    }
    return NULL;
}