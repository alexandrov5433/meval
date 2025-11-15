#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#include "data.c"

#define REGEX_MATH_EXPRESSION "^(--expression|-e)='([ A-Za-z0-9(-/^-]+)'$"
#define REGEX_RESERVED "[ep]+|ln|lg"
#define REGEX_HELP "-h|--help"
#define REGEX_VARIABLE "^(-v|--variable)=([A-Za-z]{1})=((-?[0-9]+)|(-?[0-9]+[\\.,]{1}[0-9]+))$"

double evaluate(Data *data)
{
    // check parenthesis syntax
    checkParenthesisSyntax(data);
    
    double result = 0;
    int index = 0;

    // expression: ((x - 1.45 + e * 2) + 3)^2 + 3*x - 2
    // main  ()^2 + 3*x - 2
    // sub-1  () + 3
    // sub-2  x - 1.45 + e * 2 ^ 3 / 4
    // left operant

    return result;
}

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Missing command line arguments.\nPlease run 'meval -h' for help.\n");
        exit(EXIT_FAILURE);
    }

    // meta data
    Data data = {
        .expression = {.str = NULL, .innerExpressions = NULL, .value = 0.0},
        .variables = {.array = NULL, .length = 0}};

    // help
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        // TODO: Print helpful information.
        printf("## TODO ##   Help is on the way! :D   ## TODO ##\n");
        exit(EXIT_SUCCESS);
    }

    // expression
    bool isExpressionParsed = false;
    regex_t *regexCompExp = calloc(1, sizeof(regex_t));
    size_t maxGroupsExp = 3;
    regmatch_t groupsExp[maxGroupsExp];
    if (regcomp(regexCompExp, REGEX_MATH_EXPRESSION, REG_EXTENDED))
    {
        printf("Could not compile regular expression for expression.\n");
        exit(EXIT_FAILURE);
    }

    // variable
    // p - reserved for PI
    // e - reserved for Euler
    regex_t *regexCompVar = calloc(1, sizeof(regex_t));
    size_t maxGroupsVar = 4;
    regmatch_t groupsVar[maxGroupsVar];
    if (regcomp(regexCompVar, REGEX_VARIABLE, REG_EXTENDED))
    {
        printf("Could not compile regular expression for variable.\n");
        exit(EXIT_FAILURE);
    }

    // command line arguments parsing
    for (int i = 1; i < argc; i++)
    {
        // expression
        if (!isExpressionParsed)
        {
            if (regexec(regexCompExp, argv[i], maxGroupsExp, groupsExp, 0))
            {
                printf("No expression match.\n");
                continue;
            }
            // printf("Group 0   rm_so:%d  rm_eo:%d\n", groupsExp[0].rm_so, groupsExp[0].rm_eo);
            // printf("Group 1   rm_so:%d  rm_eo:%d\n", groupsExp[1].rm_so, groupsExp[1].rm_eo);
            // printf("Group 2   rm_so:%d  rm_eo:%d\n", groupsExp[2].rm_so, groupsExp[2].rm_eo);
            int charCount = groupsExp[2].rm_eo - groupsExp[2].rm_so;
            CharArray *exp = data.expression.str;
            exp = calloc(charCount, sizeof(char));
            for (int j = 0; j < charCount; j++)
            {
                exp[j] = argv[i][groupsExp[2].rm_so + j];
            }
            printf("Expression: %s\n", exp);
            isExpressionParsed = 1;
            regfree(regexCompExp);
            continue;
        }

        // variable
        if (regexec(regexCompVar, argv[i], maxGroupsVar, groupsVar, 0) == 0)
        {
            char varChar = argv[i][groupsVar[2].rm_so];
            int varValueLength = groupsVar[3].rm_eo - groupsVar[3].rm_so;
            char *varValueStr = calloc(varValueLength + 1, sizeof(char)); // 1 additional slot for \0

            for (int j = 0; j < varValueLength; j++)
            {
                varValueStr[j] = argv[i][groupsVar[3].rm_so + j];
            }
            varValueStr[varValueLength] = '\0';
            double varValue = strtod(varValueStr, NULL);
            Variable newVar = {.name = varChar, .value = varValue};

            append(&newVar, &data.variables);

            // printf("Variable character: %c\n", varChar);
            // printf("Variable value: %lf\n", varValue);
        }
        else
        {
            printf("No match for variable.\n");
        }
    }

    double result = evaluate(&data);

    /*  for (int i = 0; i < data.varCount; i++) {
         printf("Variable: %c  Value: %f\n", varArray[i].name, varArray[i].value);
     } */

    free(data.expression.str);
    free(data.variables.array);
    
    printf("\nDone.\n");
    return EXIT_SUCCESS;
}