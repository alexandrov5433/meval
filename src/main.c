#include <stdio.h>
#include <stdlib.h>

#include "argument_parsing/_index.h"
#include "calculation/_index.h"
#include "galxlib/gstring.h"
#include "data.h"

Data *data = NULL;

void clean_up(void)
{
	free_data(data);
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		printf("Error: Missing command line arguments.\nPlease run 'meval -h' for help.\n");
		return EXIT_FAILURE;
	}

	// parsing help
	int parse_help_is_match = 0;
	int parse_help_err_status = parse_help(argv, &parse_help_is_match);
	if (parse_help_err_status)
	{
		printf(
		    "Error: parse_help returned error code %d, while parsing the command line arguments.\n",
		    parse_help_err_status);
		return EXIT_FAILURE;
	}
	if (parse_help_is_match)
		return EXIT_SUCCESS;

	// Meta data initialization and clean_up function registration.
	atexit(clean_up);
	Data *data = new_data();

	// parsing expression
	String *expression = NULL;
	int parse_expression_is_match = 0;
	int parse_err_status = parse_expression(argc, argv, &expression, &parse_expression_is_match);
	if (parse_err_status)
	{
		printf(
		    "Error: parse_expression returned error code %d, while parsing the command line arguments.\n",
		    parse_err_status);
		exit(EXIT_FAILURE);
	}
	if (parse_expression_is_match)
	{
		printf("Could not find an expression (-e|--expression) flag.\n");
		exit(EXIT_FAILURE);
	}
	if (expression == NULL)
	{
		printf("Error: Undexpened error occured. Expression pointer is NULL.\n parse_expression error code: %d.\n", parse_err_status);
		exit(EXIT_FAILURE);
	}

	int remove_space_err_status = remove_char(expression, ' ');
	if (remove_space_err_status)
	{
		printf(
		    "Error: remove_char returned error code %d, while processing the expression string.\n",
		    remove_space_err_status);
		exit(EXIT_FAILURE);
	}
	set_root_expression(expression, data);

	// parsing variables
	int parse_variable_is_match = 0;
	int parse_variable_err_status = parse_variable(data->variables, argc, argv, &parse_variable_is_match);
	if (parse_variable_err_status)
	{
		printf(
		    "Error: parse_variable returned error code %d, while parsing the command line arguments.\n",
		    parse_variable_err_status);
		exit(EXIT_FAILURE);
	}
	if (parse_variable_is_match == 0)
	{
		printf(
		    "Error: No variables (-v|--varaible) were found, while parsing the command line arguments.\n");
		exit(EXIT_FAILURE);
	}

	// expression evaluation stage
	evaluate(data);

	printf("Expression value: %.2f\n", data->expression->value);

	exit(EXIT_SUCCESS);
}