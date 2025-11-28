#ifndef REGEX_MATH_EXPRESSION
#define REGEX_MATH_EXPRESSION "^(--expression|-e)='([ A-Za-z0-9(-/^-]+)'$"
#endif

#ifndef REGEX_RESERVED
#define REGEX_RESERVED "[ep]+|ln|lg"
#endif

#ifndef REGEX_HELP
#define REGEX_HELP "^(-h|--help)$"
#endif

#ifndef REGEX_VARIABLE
#define REGEX_VARIABLE "^(-v|--variable)=([A-Za-z]{1})=((-?[0-9]+)|(-?[0-9]+[\\.,]{1}[0-9]+))$"
#endif

#ifndef REGEX_EXP_PLACEHOLDER
#define REGEX_EXP_PLACEHOLDER "^#([0-9]+)#$"
#endif
