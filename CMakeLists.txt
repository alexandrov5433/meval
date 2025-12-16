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


# TESTS

include(CTest)
# Result
add_test(NAME Test_Result_1_0 COMMAND meval "-e='2+x'" "-v=x=2")
add_test(NAME Test_Result_1_1 COMMAND meval "-e='1*(2+x)+5-2*x'" "-v=x=2")
add_test(NAME Test_Result_1_2 COMMAND meval "-e='1000000*x'" "-v=x=5")
add_test(NAME Test_Result_1_3 COMMAND meval "-e='-2*((x*(2*5))^2)'" "-v=x=2")
# Expression and variable flags
add_test(NAME Test_Flags_1_0 COMMAND meval "--expression='(x-2)^2'" "-v=x=-2")
add_test(NAME Test_Flags_1_1 COMMAND meval "-e='(x-2)^2'" "-v=x=-2")
add_test(NAME Test_Flags_1_2 COMMAND meval "-e='(x-2)^2'" "--variable=x=-2")
add_test(NAME Test_Flags_1_3 COMMAND meval "--expression='(x-2)^2'" "--variable=x=-2")
# Spacing in expression
add_test(NAME Test_Expression_Spacing_1_0 COMMAND meval "--expression='( x - 2 ) ^ 2'" "--variable=x=-2")
add_test(NAME Test_Expression_Spacing_1_1 COMMAND meval "--expression='( x - 2 ) ^2'" "--variable=x=-2")
add_test(NAME Test_Expression_Spacing_1_2 COMMAND meval "--expression='  ( x - 2 ) ^2'" "--variable=x=-2")

set(regex_result_4 [[Expression value: 4\.00]])
set(regex_result_16 [[Expression value: 16\.00]])
set(regex_result_5 [[Expression value: 5\.00]])
set(regex_result_5000000 [[Expression value: 5000000\.00]])
set(regex_result_n44 [[Expression value: -44\.00]])
# message(STATUS "REGEX regex_result_4 = [${regex_result_4}]")
# message(STATUS "REGEX regex_result_16 = [${regex_result_16}]")

set_property(
    TEST Test_Result_1_0
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_4}"
)
set_property(
    TEST Test_Result_1_1
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_5}"
)
set_property(
    TEST Test_Result_1_2
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_5000000}"
)
set_property(
    TEST Test_Result_1_3
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_n44}"
)
set_property(
    TEST
        Test_Flags_1_0
        Test_Flags_1_1
        Test_Flags_1_2
        Test_Flags_1_3
        Test_Expression_Spacing_1_0
        Test_Expression_Spacing_1_1
        Test_Expression_Spacing_1_2
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_16}"
)