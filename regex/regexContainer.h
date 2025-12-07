#include <stdlib.h>
#include <regex.h>
#include "../array/_index.h"

#ifndef REGEX_CONTAINER
#define REGEX_CONTAINER

/**
 * A container for all regex related data.
 * @param regex A ponter to the compiled regex.
 * @param maxGroups The number of gpoups, which the regex will hold.
 * @param groups A ponter to the groups specified in the regex pattern. The groups count will equal maxGroups.
 * The groups counting stats from 0 - index based. The first group (index 0) indicates the whole matchedInput string.
 * @param matchedInput A pointer to the input string, against which the compiled regex was matched. Initialized as NULL.
 * @param isMatch 1 if a match was made, 0 otherwise. Initialized as 0.
 * 
 * The matchedInput and isMatched properties are changed by the match function.
 */
typedef struct RegexContainer
{
    regex_t *regex;
    size_t maxGroups;
    regmatch_t *groups;
    char *matchedInput;
    int isMatch;
} RegexContainer;

RegexContainer *newRegexContainer(const char *pattern, size_t maxGroups, int flag);

void freeRegexContainer(RegexContainer *container);

void match(char *input, RegexContainer *container);

char *getGroupValue(const int n, const RegexContainer *container);

CharArray *getGroupValueAsCharArray(const int n, const RegexContainer *container);

#endif