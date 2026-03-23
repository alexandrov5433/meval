#include <stdio.h>
#include <stdlib.h>

#include "help.h"
#include "../regex/_index.h"

/**
 * If the string at argv[1] is the help flag, helpful information is printed in the terminal 
 * and the program exits. 
 * If there is no match, nothing is done.
 */
void parseHelp(char **argv)
{
    RegexContainer *container = newRegexContainer(REGEX_HELP, 2, REG_EXTENDED);
    match(argv[1], container);
    if (container->isMatch == 1)
    {
        // TODO: Print helpful information.
        printf("## TODO ##   Help is on the way! :D   ## TODO ##\n");
        freeRegexContainer(container);
        exit(EXIT_SUCCESS);
    }
    freeRegexContainer(container);
    return;
}