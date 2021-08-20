# Copyright (c) 2021 Oli Legat
# Licensed under the BSD 3-Clause License.
#

include(add_functions.cmake)

add_target( example_compiler
  TYPE
    executable
  SOURCES
    example_compiler.cpp
  PROPERTIES
    CXX_STANDARD   17
    CXX_EXTENSIONS OFF
)

add_target( example_runner
  TYPE
    executable
  SOURCES
    example_runner.cpp
    utils.cpp
    utils.hpp
    process.hpp
  SOURCES_UNIX
    process_posix.cpp
  PROPERTIES
    CXX_STANDARD   17
    CXX_EXTENSIONS OFF
)

add_example( basic_string
  SOURCES
    basic_string.cpp.in
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
  DEFINES
    -DCOWSTRING_IMPLICIT_STDSTRING_OPERATOR=1
)

add_example( string_swap
  SOURCES
    string_swap.cpp.in
)

add_example( string_find
  SOURCES
    string_find.cpp.in
)

write_tests_file( tests.tsv )
