#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "../array/_index.h"
#include "../regex/_index.h"

CharArray *parseExpression(const char argc, char **argv) {
    RegexContainer *container = newRegexContainer(REGEX_MATH_EXPRESSION, 3, REG_EXTENDED);
    for (int i = 1; i < argc; i++) {
        match(argv[1], container);
        if (container->isMatch == 1) {
            freeRegexContainer(container);
            return getGroupValueAsCharArray(2, container);
        }

    }
    freeRegexContainer(container);
    return NULL;
}