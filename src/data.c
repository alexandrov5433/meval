#include <stdlib.h>
#include <stdio.h>

#include "data.h"
#include "galxlib/dynamic_array.h"
#include "argument_parsing/variable.h"

/**
 * Creates a new Data structure.
 *
 * The .expression is initialized as NULL. Use the addExpressionToData function to add the root expression.
 *
 * The .variables is initialized with the newVariableArray function.
 * @return On success the pointer to the new Data, otherwise NULL.
 */
Data *new_data()
{
	Data *data = calloc(1, sizeof(Data));
	if (data == NULL)
		return NULL;

	data->expression = NULL;

	DynamicArray *var_arr = new_dynamic_array(VOID_PTR);
	if (var_arr == NULL)
	{
		free(data);
		return NULL;
	}
	data->variables = var_arr;

	return data;
}

void free_data(Data *data)
{
	if (data == NULL)
		return;
	
	freeExpression(data->expression);
	int err = process_da(data->variables, free_variable);
	free_dynamic_array(data->variables);
	free(data);
}

/**
 * Adds the root Expression to the Data structure.
 * @param exp The pointer to the CharArray, which will be used to parse the root Expression.
 * This new Expression will be set to the .expression property of the Data structure.
 * @param data The pointer to the Data structure.
 * @returns 0 on success. On failure:
 * 
 * 1 if either of the arguments are NULL.
 * 
 * 2 if the expression property of the data structure is already set.
 */
int set_root_expression(String *exp, Data *data)
{
	if (exp == NULL || data == NULL)
		return 1;

	if (data->expression != NULL)
		return 2;

	checkParenthesisSyntax(exp);
	data->expression = newExpression(exp, NULL);
	return 0;
}

/**
 * Evaluates the root expression.
 * @param data A pointer to the Data structure, containing the Expression.
 * @returns 0 on success. On failure:
 * 
 * 1 if the data argument is NULL.
 * 
 * 2 if the expression property of data is NULL.
 * 
 * 3 if the variables property of data is NULL.
 */
int evaluate(const Data *data)
{
	if (data == NULL)
		return 1;
	if (data->expression == NULL)
		return 2;
	if (data->variables == NULL)
		return 3;

	calculateExpressionValue(data->expression, data->variables);

	return 0;
}