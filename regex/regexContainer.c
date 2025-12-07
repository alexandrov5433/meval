#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "regexContainer.h"
#include "../array/_index.h"

/*
Documentation for regex.h
https://pubs.opengroup.org/onlinepubs/009696899/functions/regcomp.html
*/

/**
 * Creates a new RegexContainer.
 * @param pattern A pointer to the string pattern, which will be compiled into a regex. If NULL, NULL is returned.
 * @param maxGroups The maximum count of groups, which will be present in the regex. 
 * The first (index 0) is always the whole matched string.
 * @param flag A regex flag to use in the regcomp function. 
 * Example REG_EXTENDED (int 1) for compiling an Extended Regular Expression.
 * @return A pointer to the new RegexContainer.
 */
RegexContainer *newRegexContainer(const char *pattern, size_t maxGroups, int flag)
{
    if (pattern == NULL) {
        return NULL;
    }
    RegexContainer *container = calloc(1, sizeof(RegexContainer));
    if (container == NULL)
    {
        printf("Could allocate memory for a new RegexContainer.\n");
        exit(EXIT_FAILURE);
    }

    regex_t *regex = calloc(1, sizeof(regex_t));
    if (regex == NULL)
    {
        printf("Could allocate memory for a new regex_t.\n");
        exit(EXIT_FAILURE);
    }

    regmatch_t *groups = calloc(maxGroups, sizeof(regmatch_t));
    if (groups == NULL)
    {
        printf("Could allocate memory for a new regmatch_t.\n");
        exit(EXIT_FAILURE);
    }

    if (regcomp(regex, pattern, flag))
    {
        printf("Could not compile regular expression.\n");
        exit(EXIT_FAILURE);
    }

    container->regex = regex;
    container->maxGroups = maxGroups;
    container->groups = groups;
    container->matchedInput = NULL;
    container->isMatch = 0;

    return container;
}

/**
 * Frees the memory for the RegexContainer.
 * @param container The pointer to the RegexContainer, which is to be freed. If a NULL pointer is given, nothing is done.
 */
void freeRegexContainer(RegexContainer *container)
{
    if (container == NULL)
    {
        return;
    }
    if (container->regex != NULL)
    {
        regfree(container->regex);
    }
    if (container->groups != NULL)
    {
        free(container->groups);
    }
    free(container);
}

/**
 * Attempts to match the given string against the ragex in the RegexContainer.
 * @param input The string to match.
 * @param container The RegexContainer holding all regex related data, including the compiled regex pattern.
 *
 * If the attempt to match is successful:
 *
 * 1. the isMatch property of the container is set to 1.
 * 
 * 2. the matchedInput property of the container is set to the given input.
 *
 * On failure to match, the isMatch is set to 0 and
 * the matchedInput property is set to NULL.
 */
void match(char *input, RegexContainer *container)
{
    if (input == NULL || container == NULL)
    {
        return;
    }
    /*
    If it finds a match, regexec() shall return 0; otherwise, it shall return non-zero indicating either
    no match or an error.
    */
    int status = regexec(container->regex, input, container->maxGroups, container->groups, 0);
    container->isMatch = status == 0 ? 1 : 0;
    container->matchedInput = status == 0 ? input : NULL;
}

/**
 * Get the matched string from a group.
 * @param n The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the maxGroups property of the RegexContainer, or else NULL is returned.
 * @param container Pointer to the RegexContainer with the relevant data. If NULL, NULL is returned.
 * If the isMatch property of the container is 0, NULL is returned.
 * @returns The pointer to the string, selected by the group. 
 * The returned pointer may be used in the free function, as it's memory is manualy allocated.
 */
char *getGroupValue(const int n, const RegexContainer *container)
{
    if (container == NULL)
    {
        return NULL;
    }
    if (n < 0 || n >= container->maxGroups)
    {
        return NULL;
    }
    if (container->isMatch == 0) {
        return NULL;
    }
    int length = (container->groups)[n].rm_eo - (container->groups)[n].rm_so;
    char *value = calloc(length, sizeof(char));
    for (int i = 0; i < length; i++)
    {
        value[i] = (container->matchedInput)[(container->groups)[n].rm_so + i];
    }
    return value;
}

/**
 * Get the matched string from a group as a CharArray.
 * @param n The index of the group indicating the wanted value, from the groups property of the RegexContainer.
 * Must be 0 or less than the maxGroups property of the RegexContainer, or else NULL is returned.
 * @param container Pointer to the RegexContainer with the relevant data. If NULL, NULL is returned.
 * If the isMatch property of the container is 0, NULL is returned.
 * @returns The pointer to the CharArray containing the string, selected by the group.
 */
CharArray *getGroupValueAsCharArray(const int n, const RegexContainer *container)
{
    if (container == NULL)
    {
        return NULL;
    }
    if (n < 0 || n >= container->maxGroups)
    {
        return NULL;
    }
    if (container->isMatch == 0) {
        return NULL;
    }
    int length = (container->groups)[n].rm_eo - (container->groups)[n].rm_so;
    char *value = calloc(length, sizeof(char));
    for (int i = 0; i < length; i++)
    {
        value[i] = (container->matchedInput)[(container->groups)[n].rm_so + i];
    }
    return newCharArray(value, length);
}