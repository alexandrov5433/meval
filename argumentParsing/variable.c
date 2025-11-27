#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array/_index.c"
#include "regex/_index.c"

void parseVariable(VariableArray *varArr, const char argc, const char **argv) {
    RegexContainer *container = newRegexContainer(REGEX_VARIABLE, 6, REG_EXTENDED);
    // group indexes: 2, 3
    for (int i = 1; i < argc; i++) {
        match(argv[i], container);
        if (container->isMatch == 1) {
            char name = getLastChar(getGroupValueAsCharArray(2, container));
            double value = strtod((getGroupValueAsCharArray(3, container))->str, NULL);
            Variable *variable = newVariable(name, value);
            appendVariable(variable, varArr);
        }
    }
    freeRegexContainer(container);
}