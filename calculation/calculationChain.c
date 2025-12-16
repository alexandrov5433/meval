#include <stdio.h>
#include <math.h>

#include "calculationChain.h"
#include "operant.h"

/**
 * Creates a new CalculationChain.
 * @returns The pointer to the new CalculationChain.
 */
CalculationChain *newCalculationChain()
{
    CalculationChain *calcChain = calloc(1, sizeof(CalculationChain));
    if (calcChain == NULL)
    {
        printf("\nError: Could not allocate memory for new CalculationChain.\n\n");
        exit(EXIT_FAILURE);
    }
    calcChain->chain = NULL;
    calcChain->length = 0;
    return calcChain;
}

/**
 * Frees the memory of the CalculationChain.
 * @param calcChain The pointer to the CalculationChain, which must be freed.
 * If NULL, nothing is done.
 *
 * The Operant-s in the CalculationChain are also freed.
 */
void freeCalculationChain(CalculationChain *calcChain)
{
    if (calcChain == NULL)
    {
        return;
    }
    for (int i = 0; i < calcChain->length; i++)
    {
        freeOperant((calcChain->chain)[i]);
    }
    free(calcChain);
}

/**
 * Incerts a pointer to Operant at an index of a CalculationChain.
 * @param calcChain A pointer to the CalculationChain, in which the pointer must be incerted.
 * @param operant The pointer, which must be incerted.
 * @param i The index of the CalculationChain, at which the pointer to the Operant must be incerted.
 *
 * If either calcChain or operant are NULL, the program exits with an error.
 * If i is less than 0 or greater than the length of the CalculationChain, the program exits with an error.
 */
static void incertOperantAt(CalculationChain *calcChain, Operant *operant, int i)
{
    if (calcChain == NULL)
    {
        printf("\nError: Expected CalculationChain pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (operant == NULL)
    {
        printf("\nError: Expected Operant pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    int newChainLength = calcChain->length + 1;
    if (i < 0 || i >= newChainLength) {
            printf("\nError: Index out of bounds for incertOperantAt function.\n\n");
            exit(EXIT_FAILURE);
    }

    Operant **newChain = calloc(newChainLength, sizeof(Operant *));
    if (newChain == NULL)
    {
        printf("\nError: Could not allocate memory for Operant pointer array.\n\n");
        exit(EXIT_FAILURE);
    }

    int oldChainIndex = 0;

    for (int j = 0; j < i; j++)
    {
        newChain[j] = (calcChain->chain)[oldChainIndex];
        oldChainIndex++;
    }

    newChain[i] = operant;

    for (int j = i + 1; j < newChainLength; j++)
    {
        newChain[j] = (calcChain->chain)[oldChainIndex];
        oldChainIndex++;
    }

    free(calcChain->chain);
    calcChain->chain = newChain;
    calcChain->length = newChainLength;
}

/**
 * Deletes the Operant at i index and removes the pointer form the CalculationChain.
 * @param calcChain A pointer to the CalculationChain.
 * @param i The index at which the targeted Operator should be found.
 *
 * The length of the CalculationChain is reduced by one.
 */
static void deleteOperantAt(CalculationChain *calcChain, int i)
{
    if (calcChain == NULL)
    {
        printf("\nError: Expected CalculationChain pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (i < 0 || i >= calcChain->length)
    {
        printf("\nError: Index out of bounds for deleteOperantAt function.\n\n");
        exit(EXIT_FAILURE);
    }

    Operant *operantForDeletion = (calcChain->chain)[i];
    if (operantForDeletion == NULL)
    {
        printf("\nError: Could not find the pointer of the Operant for deletion in the CalculationChain.\n\n");
        exit(EXIT_FAILURE);
    }

    int newChainLength = calcChain->length - 1;
    if (newChainLength <= 0)
    {
        for (int i = 0; i < calcChain->length; i++)
        {
            freeOperant((calcChain->chain)[i]);
        }
        free(calcChain->chain);
        calcChain->chain = NULL;
        calcChain->length = 0;
        return;
    }

    Operant **newChain = calloc(newChainLength, sizeof(Operant *));
    if (newChain == NULL)
    {
        printf("\nError: Could not reallocate memory for Operant pointer array.\n\n");
        exit(EXIT_FAILURE);
    }

    int newChainIndex = 0;
    for (int i = 0; i < calcChain->length; i++)
    {
        Operant *current = (calcChain->chain)[i];
        if (current != operantForDeletion)
        {
            newChain[newChainIndex] = current;
            newChainIndex++;
        }
    }
    freeOperant(operantForDeletion);
    free(calcChain->chain);
    calcChain->chain = newChain;
    calcChain->length = newChainLength;
}

/**
 * Adds the given pointer (of Operant) to the end of the CalculationChain.
 * @param calcChain The pointer to the CalculationChain.
 * @param op The pointer - to an Operant -, which must be added to the end of the CalculationChail.
 *
 * @throw If either of the arguments is NULL, the program exits with EXIT_FAILURE.
 */
void appendOperantToChain(CalculationChain *calcChain, Operant *op)
{
    if (calcChain == NULL)
    {
        printf("\nError: Expected CalculationChain pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (op == NULL)
    {
        printf("\nError: Expected Operant pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    Operant **newChain = realloc(calcChain->chain, (calcChain->length + 1) * sizeof(Operant *));
    if (newChain == NULL)
    {
        printf("\nError: Could not reallocate memory for Operant pointer array.\n\n");
        exit(EXIT_FAILURE);
    }
    calcChain->chain = newChain;
    (calcChain->chain)[calcChain->length] = op;
    (calcChain->length)++;
}

/**
 * Removes two Operants from the CalculationChain and adds a third one in their place.
 * @param calcChain A pointer to the CalculationChain.
 * @param indexFirst The index of the first Operant for removal in the CalculationChain.
 * @param indexSecond The index of the second Operant for removal in the CalculationChain.
 * @param replacement A pointer to an Operant, which will be added in the CalculationChain at the indexFirst index.
 *
 * The program exits with an error:
 *
 * 1. if either calcChain or replacement are NULL.
 *
 * 2. if either indexFirst or indexSecond are less than 0 or greater than/equal to the length of the CalculationChain.
 *
 * 3. if indexFirst is equal to/greater than indexSecond.
 */
static void replaceOperants(CalculationChain *calcChain, int indexFirst, int indexSecond, Operant *replacement)
{
    if (calcChain == NULL)
    {
        printf("\nError: Expected CalculationChain pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (replacement == NULL)
    {
        printf("\nError: Expected Operant pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexFirst < 0 || indexFirst >= calcChain->length)
    {
        printf("\nError: First index out of bounds for replaceOperants function.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexSecond < 0 || indexSecond >= calcChain->length)
    {
        printf("\nError: Second index out of bounds for replaceOperants function.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexFirst == indexSecond)
    {
        printf("\nError: Indexes for replaceOperants function can not be equal.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexFirst > indexSecond)
    {
        printf("\nError: Indexes for replaceOperants function must be in the correct order.\n\n");
        exit(EXIT_FAILURE);
    }

    deleteOperantAt(calcChain, indexFirst);
    deleteOperantAt(calcChain, indexSecond - 1);
    incertOperantAt(calcChain, replacement, indexFirst);
}

/**
 * Calculates the result of the two Operants and incerts it in the CalculationChain.
 * The result - in the form of a new Operant - can be found at the indexFirst index of the CalculationChain.
 * @param calcChain A pointer to the CalculationChain. If NULL, the program exits with an error.
 * @param indexFirst The index of the first Operant in the CalculationChain used in the calculation of the result. Must be greater than 0 and with 1 less than indexSecond.
 * @param indexSecond The index of the second Operant in the CalculationChain used in the calculation of the result. Must be greater than 0 and less than the length of the CalculationChain.
 */
void mergeOperants(CalculationChain *calcChain, int indexFirst, int indexSecond)
{
    if (calcChain == NULL)
    {
        printf("\nError: Expected CalculationChain pointer, received NULL.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexFirst < 0 || indexSecond >= calcChain->length || indexSecond < 0)
    {
        printf("\nError: Index out of bounds for CalculationChain.\n\n");
        exit(EXIT_FAILURE);
    }
    if (indexFirst != indexSecond - 1)
    {
        printf("\nError: The merging of Operants in the CalculationChain requires them to be on neighbouring indexes.\n\n");
        exit(EXIT_FAILURE);
    }

    Operant *left = (calcChain->chain)[indexFirst];
    Operant *right = (calcChain->chain)[indexSecond];
    double result;

    switch (right->operator)
    {
    case '^':
        result = pow(left->value, right->value);
        break;
    case '*':
        result = (left->value) * (right->value);
        break;
    case ':':
        result = (left->value) / (right->value);
        break;
    case '+':
        result = (left->value) + (right->value);
        break;
    case '-':
        if (right->value < 0)
        {
            result = (left->value) + (right->value);
        }
        else
        {
            result = (left->value) - (right->value);
        }
        break;
    }

    replaceOperants(calcChain, indexFirst, indexSecond, newOperantFromDouble(result));
}
