#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "../array/_index.h"
#include "calculationChain.h"

/**
 * Deletes the pointer to the Expression from the ExpressionArray, if present.
 * @param exp A pointer to the Expression, which must be deleted from the array.
 * If this pointer is not present in the ExpressionArray, the (same) pointer to the ExpressionArray is returned.
 * If NULL, nothing is done and the (same) pointer given for the ExpressionArray is returned.
 * Even if removed from the array, the memory of this pointer is NOT freed.
 * @param arr A pointer to the ExpressionArray, from which the pointer to the Expression must be deleted.
 * If NULL, nothing is done and NULL is returned.
 * The memory of this (old) pointer is freed:
 *
 * 1. when the new one is returned.
 *
 * 2. when there was only one element in the ExpressionArray, which was deleted.
 *
 * @return A new pointer to the newly created (without the targeted Expression pointer) ExpressionArray.
 * The length of the new array will be with one less than that of the targeted ExpressionArray.
 * If the new array is empty (e.g. the targeted ExpressionArray contained only 1 pointer, which was to be deleted), NULL is returned.
 */
ExpressionArray *deleteExpFromArray(Expression *exp, ExpressionArray *arr)
{
    if (exp == NULL)
    {
        return arr;
    }
    if (arr == NULL)
    {
        return NULL;
    }

    int isPresent = 0;
    for (int i = 0; i < arr->length; i++)
    {
        if (exp == (arr->array)[i])
        {
            isPresent = 1;
            break;
        }
    }
    if (isPresent == 0)
    {
        return arr;
    }

    int newLength = (arr->length) - 1;
    if (newLength <= 0)
    {
        free(arr);
        return NULL;
    }

    ExpressionArray *newExpArr = calloc(1, sizeof(ExpressionArray));
    if (newExpArr == NULL)
    {
        printf("\nError: Could not allocate memory for a new ExpressionArray.\n\n");
        exit(EXIT_FAILURE);
    }

    Expression **newArr = calloc(newLength, sizeof(Expression *));
    if (newArr == NULL)
    {
        printf("\nError: Could not allocate memory for a new Array for pointers. Func: deleteExpFromArray.\n\n");
        exit(EXIT_FAILURE);
    }

    newExpArr->array = newArr;
    newExpArr->length = newLength;

    int index = 0;
    for (int i = 0; i < arr->length; i++)
    {
        if ((arr->array)[i] != exp)
        {
            (newExpArr->array)[index] = (arr->array)[i];
            index++;
        }
    }

    free(arr);
    return newExpArr;
}

/**
 * This is a helper function for the freeExpression function.
 * This lets us remove the pointer of the target Expression from the innerExpressions array of the parent Expression, if present.
 */
static void _freeExpression(Expression *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    if (ptr->innerExpressions != NULL)
    {
        for (int i = 0; i < ptr->innerExpressions->length; i++)
        {
            _freeExpression((ptr->innerExpressions->array)[i]);
        }
        free(ptr->innerExpressions);
    }
    free(ptr);
}

/**
 * Frees the memory allocated for the Expression and all inner expressions in the ExpressionArray, if present.
 * The Expression in the .parentExpression, if present, property is not touched.
 * @param ptr A pointer to the Expression, from which to start freeing memory. If NULL, nothing is done.
 */
void freeExpression(Expression *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    // remove the pointer of this Expression from the parent Expression, if present.
    if (ptr->parentExpression != NULL)
    {
        ptr->parentExpression->innerExpressions = deleteExpFromArray(ptr, ptr->parentExpression->innerExpressions);
    }
    _freeExpression(ptr);
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

    mainExp->innerExpressions->array = newArray;
    (mainExp->innerExpressions->array)[mainExp->innerExpressions->length] = innerExp; // old length is now the last index
    mainExp->innerExpressions->length++;                                              // increment old length, to actually show length
}

/**
 * Creates a CharArray placeholder in the form: '#n#'.
 * @param n Pointer to an integer, the value of which will be placed in the placeholder.
 */
static CharArray *createPlaceholder(int *n)
{
    // INT_MAX == 2147483647 == 10 char ; 2 '#' char; '\0' char from sprintf
    char *buffer = calloc(13, sizeof(char));
    sprintf(buffer, "#%d#", *n);
    int placeholderLength = 0;
    for (int i = 1; i < 13; i++)
    {
        // index 0 is always '#'
        if (buffer[i] == '#')
        {
            placeholderLength = i + 1;
            break;
        }
    }
    CharArray *placeholder = newCharArray(buffer, placeholderLength);
    squish(placeholder);
    free(buffer);
    return placeholder;
}

static void extractExpression(Expression *mainExp, int openIndex, int closeIndex, int *innerExpCounter)
{
    CharArray *placeholder = createPlaceholder(innerExpCounter);

    CharArray *innerExp = copy(mainExp->exp, openIndex + 1, closeIndex - 1); // +1 and -1 to not copy the '(' and ')'
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

void calculateExpressionValue(Expression *expression, VariableArray *variables)
{
    if (expression->innerExpressions != NULL)
    {
        for (int i = 0; i < expression->innerExpressions->length; i++)
        {
            calculateExpressionValue((expression->innerExpressions->array)[i], variables);
        }
    }

    // build CalculationChain
    // 2+#0#^2+#1#*2
    CalculationChain *calcChain = newCalculationChain();
    int markerIndex = 0;
mainLoop:
    while (markerIndex < expression->exp->length)
    {
        Operant *operant = newOperant();

        // operator
        char o = (expression->exp->str)[markerIndex];
        if (o == '+' || o == '-' || o == '*' || o == '/' || o == '^')
        {
            operant->operator = o;
            markerIndex++;
        }

        // operant
        // case: inner expression placeholder: #0#
        if ((expression->exp->str)[markerIndex] == '#')
        {
            operant->isExpression = 1;
            operant->isVariable = 0;
            appendCharToOperant('#', operant); // opening #

            for (int i = markerIndex + 1; i < expression->exp->length; i++)
            {
                char c = (expression->exp->str)[i];
                if (c >= 48 && c <= 57)
                {
                    appendCharToOperant(c, operant); // 0-9
                }
                else if (c == '#')
                {
                    appendCharToOperant('#', operant); // closing #
                    markerIndex = i + 1;
                    break;
                }
            }
        }
        else if ((expression->exp->str)[markerIndex] > 96 && (expression->exp->str)[markerIndex] < 123)
        {
            // case: variable
            // 'a' == 97 ; 'z' == 122
            operant->isExpression = 0;
            operant->isVariable = 1;
            appendCharToOperant((expression->exp->str)[markerIndex], operant);
            markerIndex++;
        }
        else
        {
            // case: integer or double
            operant->isExpression = 0;
            operant->isVariable = 0;
            for (int i = markerIndex; i < expression->exp->length; i++)
            {
                char c = (expression->exp->str)[i];
                if (c >= 48 && c <= 57)
                {
                    appendCharToOperant(c, operant); // 0-9
                    if (i + 1 < expression->exp->length)
                    {
                        char nextChar = (expression->exp->str)[i + 1];
                        if (nextChar < 48 || nextChar > 57)
                        {
                            checkOperantEnd(operant);
                            markerIndex = i + 1;
                            evaluateOperantValue(expression, variables, operant);
                            appendOperantToChain(calcChain, operant);
                            goto mainLoop;
                        }
                    }
                    else
                    {
                        checkOperantEnd(operant);
                        markerIndex = i + 1;
                    }
                }
                else if (c == '.' || c == ',')
                {
                    checkFloatingPointIncluded(operant); // exits if '.' or ',' is already included.
                    appendCharToOperant(c, operant);
                    operant->floatingPointSymbolIncluded = 1;
                }
                else
                {
                    checkOperantEnd(operant); // exits if '.' or ',' is the last character
                    markerIndex = i;
                    break;
                }
            }
        }

        evaluateOperantValue(expression, variables, operant);
        appendOperantToChain(calcChain, operant);
    }

    // calculate expression value from CalculationChain
    // Operant **chain = calcChain->chain;
    int *chainLength = &(calcChain->length);

    // operator: ^
    for (int exponentIndex = 0; exponentIndex < *chainLength; exponentIndex++)
    {
        Operant *currentOperant = (calcChain->chain)[exponentIndex];
        if (currentOperant->operator == '^')
        {
            /*
                First Operant in chain can not be with ^ operator.
                There will always be a previous operant.
            */
            mergeOperants(calcChain, exponentIndex - 1, exponentIndex);
            exponentIndex = 0;
        }
    }

    // operator: *
    for (int multiplicationIndex = 0; multiplicationIndex < *chainLength; multiplicationIndex++)
    {
        Operant *currentOperant = (calcChain->chain)[multiplicationIndex];
        if (currentOperant->operator == '*')
        {
            /*
                First Operant in chain can not be with * operator.
                There will always be a previous operant.
            */
            mergeOperants(calcChain, multiplicationIndex - 1, multiplicationIndex);
            multiplicationIndex = 0;
        }
    }

    // operator: /
    for (int divisionIndex = 0; divisionIndex < *chainLength; divisionIndex++)
    {
        Operant *currentOperant = (calcChain->chain)[divisionIndex];
        if (currentOperant->operator == '/')
        {
            /*
                First Operant in chain can not be with / operator.
                There will always be a previous operant.
            */
            mergeOperants(calcChain, divisionIndex - 1, divisionIndex);
            divisionIndex = 0;
        }
    }

    // operator: +
    for (int additionIndex = 1; additionIndex < *chainLength; additionIndex++)
    {
        Operant *currentOperant = (calcChain->chain)[additionIndex];
        if (currentOperant->operator == '+')
        {
            /*
                Starting from index 1 to prevent errors.
                The first operator may have '+', '-' or '\0' as a value for the operator property.
                If only one Operant is left in the CalculationChain, no futher calculations are needed. This one Operant is the end result.
            */
            mergeOperants(calcChain, additionIndex - 1, additionIndex);
            additionIndex = 1;
        }
    }

    // operator: -
    for (int subtractionIndex = 1; subtractionIndex < *chainLength; subtractionIndex++)
    {
        Operant *currentOperant = (calcChain->chain)[subtractionIndex];
        if (currentOperant->operator == '-')
        {
            /*
                Starting from index 1 to prevent errors.
                The first operator may have '+', '-' or '\0' as a value for the operator property.
                If only one Operant is left in the CalculationChain, no futher calculations are needed. This one Operant is the end result.
            */
            mergeOperants(calcChain, subtractionIndex - 1, subtractionIndex);
            subtractionIndex = 1;
        }
    }

    expression->value = ((calcChain->chain)[0])->value;
    freeCalculationChain(calcChain);
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
    newExp->innerExpressions = NULL;
    newExp->parentExpression = parentExpression; // can be NULL
    newExp->value = 0.0;

    parse(newExp);
    return newExp;
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