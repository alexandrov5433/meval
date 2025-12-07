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
    switch (right->operator)
    {
    case '^':
        Operant *newOp= newOperant();
        double result = pow(left->value, right->value);
        
        break;

    case '*':
        /* code */
        break;

    case ':':
        /* code */
        break;

    case '+':
        /* code */
        break;
    case '-':
        /* code */
        break;

    default:
        break;
    }
}
