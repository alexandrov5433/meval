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

static void incertOperantAt(CalculationChain *calcChain, const Operant *operant, int i);

static void deleteOperantAt(CalculationChain *calcChain, int i);

void appendOperantToChain(CalculationChain *calcChain, Operant *op);

static void replaceOperants(CalculationChain *calcChain, int indexFirst, int indexSecond, Operant *replacement);

void mergeOperants(CalculationChain *calcChain, int indexFirst, int indexSecond);

#endif