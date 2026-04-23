#include <stdlib.h>

#include "help.h"
#include "galxlib/gregex.h"
#include "regex_definitions.h"

/**
 * If the string at argv[1] is the help flag, helpful information is printed in the terminal.
 * If there is no match, nothing is done.
 * @param argv The array of strings to be matched against.
 * @param output_is_match An integer pointer. If there is a match it is set to 1, otherwise to 0.
 * @returns 0 on successful execution. Not matching is NOT failure! On failure:
 *
 * 1 on a RegexContainer initialization error.
 *
 * 2 if match (from galxlib/gregex) retuns an error code.
 */
int parse_help(char **argv, int *output_is_match)
{
	int cont_init_err = 0;
	RegexContainer *container = new_regex_container(REGEX_HELP, 2, REG_EXTENDED, &cont_init_err);
	if (cont_init_err || container == NULL)
		return 1;

	if (match(argv[1], container))
	{
		free_regex_container(container);
		return 2;
	}

	*output_is_match = container->isMatch;
	// TODO: Print helpful information.
	if (container->isMatch == 1)
		printf("## TODO ##   Help is on the way! :D   ## TODO ##\n");

	free_regex_container(container);
	return 0;
}