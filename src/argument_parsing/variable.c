#include <stdlib.h>

#include "variable.h"
#include "regex_definitions.h"
#include "galxlib/dynamic_array.h"
#include "galxlib/gregex.h"

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
 * @param var_arr A pointer to the DynamicArray where the poniters to the newly initialized Variables will be placed.
 * @param argc The count of arguments in argv.
 * @param argv The character array with the arguments.
 * @param output_is_match Is set to 1 if at least one Variable was matched, otherwise to 0.
 * @returns 0 on successful execution. Not matching is NOT failure. On failure:
 *
 * 1 on a RegexContainer initialization error.
 *
 * 2 if match (from galxlib/gregex) retuns an error code.
 */
int parse_variable(DynamicArray *var_arr, const int argc, char **argv, int *output_is_match)
{
	// group indexes: 2, 3
	int cont_err_status = 0;
	RegexContainer *container = new_regex_container(REGEX_VARIABLE, 6, REG_EXTENDED, &cont_err_status);
	if (cont_err_status || container == NULL)
		return 1;

	int is_match = 0;
	for (size_t i = 1; i < argc; i++)
	{
		// match returns an error code
		if (match(argv[i], container))
		{
			free_regex_container(container);
			return 2;
		}

		if (container->isMatch == 1)
		{
			is_match = 1;
			String *tmp = get_group_value_str(2, container);
			char name = (tmp->str)[tmp->length - 1];
			free_string(tmp);

			String *str = get_group_value_str(3, container);
			char *str_nt = NULL;
			int err_str = get_raw_nt(str, &str_nt);
			if (err_str != 0 || str_nt == NULL)
			{
				free_string(str);
				return 3;
			}
			free_string(str);

			double value = strtod(str_nt, NULL);
			/*
			The memory for this char[] was allocated manually by get_raw_nt
			and can therefore be freed.
			*/
			free(str_nt);

			Variable *var = new_variable(name, value);
			if (var == NULL)
				return 4;

			int err_da = push_da(var_arr, var);
			if (err_da != 0)
			{
				free_variable(var);
				return 5;
			}
		}
	}
	*output_is_match = is_match;
	free_regex_container(container);
	return 0;
}