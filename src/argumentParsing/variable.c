#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable.h"
#include "../regex/_index.h"
#include "galxlib/dynamic_array.h"

/**
 * Creates a new Varaible.
 * @param name The value for the name property of the Variable.
 * @param value The value for the value property of the Variable.
 * @return The pointer to the newly created Variable.
 */
Variable *new_variable(const char name, const double value)
{
	Variable *variable = calloc(1, sizeof(Variable));
	if (variable == NULL)
		return NULL;

	variable->name = name;
	variable->value = value;
	return variable;
}

/**
 * Frees the memory for the Variable instance.
 * @param var A pointer to the Variable to free. If NULL, nothing is done.
 */
inline void free_variable(Variable *var)
{
	if (var == NULL)
		return;
	free(var);
}

/**
 * Parses the variables from argv.
 * @returns 0 on success, 1 on failure.
 */
int parse_variable(DynamicArray *var_arr, const char argc, char **argv)
{
	// group indexes: 2, 3
	RegexContainer *container = newRegexContainer(REGEX_VARIABLE, 6, REG_EXTENDED);

	for (int i = 1; i < argc; i++)
	{
		match(argv[i], container);
		if (container->isMatch == 1)
		{
			String *tmp = getGroupValueAsString(2, container);
			char name = (tmp->str)[tmp->length - 1];
			free_string(tmp);

			String *str = getGroupValueAsString(3, container);
			char *str_nt = NULL;
			int err_str = get_raw_nt(str, &str_nt);
			if (err_str != 0 || str_nt == NULL)
			{
				free_string(str);
				return 1;
			}
			free_string(str);

			double value = strtod(str_nt, NULL);

			Variable *var = new_variable(name, value);
			if (var == NULL)
				return 1;

			int err_da = push_da(var_arr, var);
			if (err_da != 0)
			{
				free_variable(var);
				return 1;
			}
		}
	}
	freeRegexContainer(container);
	return 0;
}