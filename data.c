#include <stdlib.h>
#include <stdio.h>

#include "array/_index.c"
#include "calculation/_index.c"
#include "regex/_index.c"

typedef struct Data
{
    Expression *expression;
    VariableArray *variables;
} Data;

/**
 * Creates a new Data struncture.
 * 
 * The .expression is initialized as NULL. Use the addExpressionToData function to add the root expression.
 * 
 * The .variables is initialized with the newVariableArray function. 
 * @return The pointer to the new Data.
 */
Data *newData() {
    Data *data = calloc(1, sizeof(Data));
    if (data == NULL) {
        printf("\nError: Could not allocate memory for a new Data.\n\n");
        exit(EXIT_FAILURE);
    }
    
    data->expression = NULL;
    data->variables = newVariableArray();
    
    return data;
}

/**
 * Adds the root Expression to the Data structure.
 * @param exp The pointer to the CharArray, which will be used to parse the root Expression. 
 * This new Expression will be set to the .expression property of the Data structure.
 * @param data The pointer to the Data structure.
 * 
 * The program exits with EXIT_FAILURE:
 * 
 * 1. If either of the function arguments are NULL.
 * 
 * 2. If the Data structure already has a set root Expression.
 */
void setRootExpression(CharArray *exp, Data *data) {
    if (exp == NULL || data == NULL) {
        printf("\nError: NULL pointers are not acceptable arguments for the setRootExpression function.\n\n");
        exit(EXIT_FAILURE);
    }
    if (data->expression != NULL) {
        printf("\nError: The given Data structure already has a set expression property.\n\n");
        exit(EXIT_FAILURE);
    }
    checkParenthesisSyntax(exp);
    data->expression = newExpression(exp, NULL);
}

/**
 * Evaluates the root expression.
 * @param data A pointer to the Data structure, containing the Expression.
 * 
 * The program exits with EXIT_FAILURE if either of the .expression and .variables properties of the Data structure are NULL.
 * 
 */
void evaluate(Data *data)
{
    if (data->expression == NULL) {
        printf("\nError: The given Data structure has a NULL pointer for an .expression property.\n\n");
        exit(EXIT_FAILURE);
    }
    if (data->variables == NULL) {
        printf("\nError: The given Data structure has a NULL pointer for a .variables property.\n\n");
        exit(EXIT_FAILURE);
    }
    // check parenthesis syntax
    calculateExpressionValue(data->expression, data->variables);
    
}