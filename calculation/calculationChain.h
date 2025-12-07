#include "operant.h"

#ifndef CALCULATION_CHAIN
#define CALCULATION_CHAIN

typedef struct CalculationChain
{
    struct Operant **chain;
    int length;
} CalculationChain;

CalculationChain *newCalculationChain();

void freeCalculationChain(CalculationChain *calcChain);

void appendOperantToChain(CalculationChain *calcChain, Operant *op);

#endif