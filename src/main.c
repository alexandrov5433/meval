#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "regex/_index.h"
#include "argumentParsing/_index.h"
#include "array/_index.h"
#include "calculation/_index.h"
#include "data.h"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Missing command line arguments.\nPlease run 'meval -h' for help.\n\n");
        exit(EXIT_FAILURE);
    }

    // help
    parseHelp(argv);

    // meta data
    Data *data = newData();

    // expression
    CharArray *expression = parseExpression(argc, argv);
    squish(expression);
    if (expression == NULL) {
        printf("Could not find an expression (-e|--expression) flag.\n");
        exit(EXIT_FAILURE);
    }
    setRootExpression(expression, data);
    
    // variable
    // TODO: Handle case where the are no variables in the expression.
    parseVariable(data->variables, argc, argv);
    if (data->variables->array == NULL) {
        printf("Could not find variable (-v|--variable) flags.\n");
        exit(EXIT_FAILURE);
    }
    
    evaluate(data);

    printf("Expression value: %.2f\n", data->expression->value);

    freeData(data);

    return EXIT_SUCCESS;
}