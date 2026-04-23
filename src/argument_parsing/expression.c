#include <stdlib.h>

#include "expression.h"
#include "regex_definitions.h"
#include "galxlib/gregex.h"
#include "galxlib/gstring.h"

/**
 * Parses the expression in argv, if found, and places the matched String in the output.
 * @param argc The count of arguments in argv.
 * @param argv The character array with the arguments.
 * @param output_expression A double String pointer where the parsed expression will be placed.
 * @returns 0 on successful execution. Not matching is NOT failure. On failure:
 * 
 * 1 on a RegexContainer initialization error.
 *
 * 2 if match (from galxlib/gregex) retuns an error code.
 */
int parse_expression(const int argc, char **argv, String **output_expression, int *output_is_match)
{
	int cont_err_status = 0;
	RegexContainer *container = new_regex_container(REGEX_MATH_EXPRESSION, 3, REG_EXTENDED, &cont_err_status);
	if (cont_err_status || container == NULL)
		return 1;

	int is_match = 0;
	for (size_t i = 1; i < argc; i++)
	{
		if (match(argv[1], container))
		{
			free_regex_container(container);
			return 2;
		}

		if (container->isMatch == 1)
		{
			*output_expression = get_group_value_str(2, container);
			if (output_expression == NULL)
			{
				free_regex_container(container);
				return 3;
			}
			break;
		}
	}

	*output_is_match = container->isMatch;
	free_regex_container(container);
	return 0;
}