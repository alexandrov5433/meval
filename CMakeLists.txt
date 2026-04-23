cmake_minimum_required(VERSION 3.20)

project(meval VERSION 1.0.0 LANGUAGES C)

add_executable(meval)

target_sources(meval
    PRIVATE
        src/main.c
        src/data.c

        src/argument_parsing/expression.c
        src/argument_parsing/help.c
        src/argument_parsing/variable.c

        src/calculation/calculation_chain.c
        src/calculation/expression.c
        src/calculation/operant.c
        src/calculation/parenthesis.c
)

target_include_directories(meval
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}

        argument_parsing
        array
        calculation
)

# Compile lib code with -g flag, enabling debugging of lib_test.exe with working breakpoints
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build." FORCE)
endif()

# Link galxlib library. Translates to compilation with -lgalxlib flag.
# Galxlib is a custom library and must be installed manually. https://github.com/alexandrov5433/glib
target_link_libraries(meval galxlib)

# Link math library. Translates to compilaition with -lm flag.
target_link_libraries(meval m)

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
# run 'ctest' in build directory to execute tests

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
set(regex_result_n800 [[Expression value: -800\.00]])
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
    PROPERTY PASS_REGULAR_EXPRESSION "${regex_result_n800}"
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