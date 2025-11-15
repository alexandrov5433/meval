#include <stdio.h>
#include <stdlib.h>

#include "array/_index.c"

typedef struct Expression
{
    CharArray *exp;
    ExpressionArray *innerExpressions;
    Expression *parentExpression;
    double value;
} Expression;

typedef struct ExpressionArray
{
    Expression **array;
    size_t length;
} ExpressionArray;

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
    Expression *newExp = calloc(sizeof(Expression), 1);
    if (newExp == NULL)
    {
        printf("\nError: Could not allocate memory for a new Expression.\n\n");
        exit(EXIT_FAILURE);
    }
    newExp->exp = expression;
    newExp->innerExpressions = NULL;
    newExp->parentExpression = parentExpression; // can be NULL
    newExp->value = 0.0;

    parse(newExp);
}

static ExpressionArray *newExpressionArray(Expression *exp)
{
    // TODO: NULL-check for exp ???
    ExpressionArray *expArray = calloc(sizeof(ExpressionArray), 1);
    if (expArray == NULL)
    {
        printf("\nError: Could not allocate memory for a new ExpressionArray.\n\n");
        exit(EXIT_FAILURE);
    }

    Expression **array = calloc(sizeof(Expression *), 1);
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
    char *buffer = calloc(sizeof(char), 12);
    sprintf(buffer, "#%d#", *n);
    CharArray *placeholder = newCharArray(buffer, 19);
    squish(placeholder);
    free(buffer);
}

static void extractExpression(Expression *mainExp, int openIndex, int closeIndex, int *innerExpCounter)
{
    CharArray *placeholder = createPlaceholder(innerExpCounter);

    // TODO: What to do when we have '()' in the expression??? Handle in parenthesis syntax check? Syntax Error?
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
}