cmake_minimum_required(VERSION 3.20)

project(meval VERSION 1.0.0 LANGUAGES C)

add_executable(meval)

target_sources(meval
    PRIVATE
        main.c
        data.c

        argumentParsing/expression.c
        argumentParsing/help.c
        argumentParsing/variable.c

        array/charArray.c
        array/intArray.c
        array/variableArray.c

        calculation/calculationChain.c
        calculation/expression.c
        calculation/operant.c
        calculation/parenthesis.c

        regex/regexContainer.c
)

target_include_directories(meval
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}

        argumentParsing
        array
        calculation
        regex
)

include(CheckLibraryExists)

check_library_exists(regex regcomp "" HAVE_LIBREGEX)

if (HAVE_LIBREGEX)
    target_link_libraries(meval PRIVATE regex)
endif()

if (MSVC)
    target_compile_options(meval PRIVATE /W4)
else()
    target_compile_options(meval PRIVATE -Wall -Wextra -Wpedantic)
endif()


include(CTest)

add_test(NAME Test_Expression_1 COMMAND meval "-e='2+x'" "-v=x=2")
add_test(NAME Test_Expression_2 COMMAND meval "--expression='(x - 2)^2'" "-v=x=-2")

set(regex_t_e_1 [[Expression value: 4\.00]])
set(regex_t_e_2 [[Expression value: 16\.00]])
message(STATUS "REGEX regex_t_e_1 = [${regex_t_e_1}]")
message(STATUS "REGEX regex_t_e_2 = [${regex_t_e_2}]")

set_property(
    TEST Test_Expression_1
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_t_e_1}"
)
set_property(
    TEST Test_Expression_2
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_t_e_2}"
)