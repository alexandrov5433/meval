#include <stdio.h>
#include <stdlib.h>

#include "array/_index.c"

typedef struct Expression
{
    CharArray *exp;
    CharArray *expDummy;
    ExpressionArray *innerExpressions;
    Expression *parentExpression;
    double value;
} Expression;

typedef struct ExpressionArray
{
    Expression **array;
    size_t length;
} ExpressionArray;

void updateDummy(Expression *expression) {
    freeCharArray(expression->expDummy);
    expression->expDummy = copy(expression->exp, 0, expression->exp->length - 1);
}

static void addInnerExpression(Expression *mainExp, Expression *innerExp)
{
    if (mainExp->innerExpressions == NULL)
    {
        mainExp->innerExpressions = newExpressionArray(innerExp);
        return;
    }
    Expression **innerExpArr = mainExp->innerExpressions->array;

    Expression **newArray = realloc(innerExpArr, mainExp->innerExpressions->length + 1);
    if (newArray == NULL)
    {
        printf("\nError: Could not reallocate memory, in order to extend an array for Expression-s.\n\n");
        exit(EXIT_FAILURE);
    }

    *innerExpArr = newArray;
    innerExpArr[mainExp->innerExpressions->length] = innerExp; // old length is now the last index
    mainExp->innerExpressions->length++; // increment old length, to actually show length
}

/**
 * Creates a CharArray placeholder in the form: '#n#'.
 * @param n Pointer to an integer, the value of which will be placed in the placeholder.
 */
static CharArray *createPlaceholder(int *n)
{
    // INT_MAX == 2147483647 == 10 char
    char *buffer = calloc(12, sizeof(char));
    sprintf(buffer, "#%d#", *n);
    CharArray *placeholder = newCharArray(buffer, 19);
    squish(placeholder);
    free(buffer);
}

static void extractExpression(Expression *mainExp, int openIndex, int closeIndex, int *innerExpCounter)
{
    CharArray *placeholder = createPlaceholder(innerExpCounter);

    CharArray *innerExp = copy(mainExp, openIndex + 1, closeIndex - 1); // +1 and -1 to not copy the '(' and ')'
    replacePart(mainExp->exp, openIndex, closeIndex, placeholder);
    freeCharArray(placeholder);
    
    Expression *newInnerExp = newExpression(innerExp, mainExp);
    addInnerExpression(mainExp, newInnerExp);
}

static void parse(Expression *mainExp)
{
    /*
    2 + ((x - 1)*(2 + 4))^2 + (e + x)*2
        ^---------------^     ^-----^
        first exp = #0#       second exp = #1#      =>      2 + #0#^2 + #1#*2
    The '^' character underlines the opening and closing indexes of the inner expression.
    Opening and closing parenthesis indicate the start and end of an inner expression respectively.
    The inner expression will not include the characters of the opening/closing indexes - '(' and ')'.
    The n in '#n#' - placeholder for the inner in the main expression - indicates the position of the inner expression in the main expression.
    Counting for n starts from 0.
    #0# == (x - 1)*(2 + 4)
    #1# == e + x
    */

    int openIndex = -1; // left #
    // int closeIndex = -1;    // right #
    int newOpeningParenthesisCount = 0; // needed in order to skip over inner expressions while searching for the closing parenthesis
    int innerExpCounter = 0;

    for (int i = 0; i < mainExp->exp->length; i++)
    {
        char c = (mainExp->exp->str)[i];
        if (c == '(')
        {
            if (openIndex == -1)
            {
                openIndex = i;
            }
            else
            {
                newOpeningParenthesisCount++;
            }
        }
        else if (c == ')')
        {
            if (newOpeningParenthesisCount > 0)
            {
                newOpeningParenthesisCount--;
            }
            else
            {
                extractExpression(mainExp, openIndex, i, &innerExpCounter); // closeIndex == i;
                innerExpCounter++;
                i = openIndex; // saves some cicles; can not continue from closeIndex, because the placeholder chages the length.
            }
        }
    }

    updateDummy(mainExp);
}

void calculateExpressionValue(Expression *expression, VariableArray *variables)
{
    if (expression->innerExpressions != NULL) {
        for (int i = 0; i < expression->innerExpressions->length; i++) {
            calculateExpressionValue((expression->innerExpressions->array)[i], variables);
        }
    }

    // 2 + #0#^2 + #1#*2
    char *expStr = expression->expDummy->str;
    int *expLength = &(expression->expDummy->length);

    // operator: ^
    for (int exponentIndex = 0; exponentIndex < *expLength; exponentIndex++)
    {
        char currentChar = expStr[exponentIndex];
        if (currentChar == '^')
        {
            calcOperator(expression, variables, exponentIndex);
            exponentIndex = 0;
        }
    }
    // operator: *
    for (int multiplicationIndex = 0; multiplicationIndex < *expLength; multiplicationIndex++)
    {
        char currentChar = expStr[multiplicationIndex];
        if (currentChar == '*')
        {
            calcOperator(expression, variables, multiplicationIndex);
            multiplicationIndex = 0;
        }
    }
    // operator: /
    for (int divisionIndex = 0; divisionIndex < *expLength; divisionIndex++)
    {
        char currentChar = expStr[divisionIndex];
        if (currentChar == '/')
        {
            calcOperator(expression, variables, divisionIndex);
            divisionIndex = 0;
        }
    }
    // operator: +
    for (int additionIndex = 0; additionIndex < *expLength; additionIndex++)
    {
        char currentChar = expStr[additionIndex];
        if (currentChar == '+')
        {
            calcOperator(expression, variables, additionIndex);
            additionIndex = 0;
        }
    }
    // operator: -
    for (int subtractionIndex = 0; subtractionIndex < *expLength; subtractionIndex++)
    {
        char currentChar = expStr[subtractionIndex];
        if (currentChar == '-')
        {
            calcOperator(expression, variables, subtractionIndex);
            subtractionIndex = 0;
        }
    }

    // TODO: add logarithm, math. constants etc.

    // at this point, there should be only one number in the expression's string - the result
    expression->value = strtod(expStr, NULL);
}

/**
 * Creates a new Expression.
 * @param expression Pointer to a CharArray containing the expression, which is the main expression for this instance.
 * The memory of this pointer must not be freed, as this pointer is  placed in the new Expression instance. 
 * @param parentExpression Pointer to an other Expression, in which this new one will be contained.
 * The memory of this pointer must also not be freed. NULL can be given as an argument, if there is no parent Expression.
 * @return The pointer to the newly created Expression instance.
 */
Expression *newExpression(CharArray *expression, Expression *parentExpression)
{
    Expression *newExp = calloc(1, sizeof(Expression));
    if (newExp == NULL)
    {
        printf("\nError: Could not allocate memory for a new Expression.\n\n");
        exit(EXIT_FAILURE);
    }
    newExp->exp = expression;
    newExp->expDummy = copy(expression, 0, expression->length - 1);
    newExp->innerExpressions = NULL;
    newExp->parentExpression = parentExpression; // can be NULL
    newExp->value = 0.0;

    parse(newExp);
}

static ExpressionArray *newExpressionArray(Expression *exp)
{
    // TODO: NULL-check for exp ???
    ExpressionArray *expArray = calloc(1, sizeof(ExpressionArray));
    if (expArray == NULL)
    {
        printf("\nError: Could not allocate memory for a new ExpressionArray.\n\n");
        exit(EXIT_FAILURE);
    }

    Expression **array = calloc(1, sizeof(Expression *));
    if (array == NULL)
    {
        printf("\nError: Could not allocate memory for a new array for Expression-s.\n\n");
        exit(EXIT_FAILURE);
    }
    array[0] = exp;

    expArray->array = array;
    expArray->length = 1;

    return expArray;
}