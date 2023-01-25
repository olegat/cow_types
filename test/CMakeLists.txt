# Copyright (c) 2021 Oli Legat
# Licensed under the BSD 3-Clause License.
#

include(add_functions.cmake)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

add_target( example_compiler
  TYPE
    executable
  SOURCES
    example_compiler.cpp
  PROPERTIES
    CXX_STANDARD   17
    CXX_EXTENSIONS OFF
    FOLDER         "test"
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:example_compiler>
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
  SOURCES_WIN32
    process_win32.cpp
  PROPERTIES
    CXX_STANDARD   17
    CXX_EXTENSIONS OFF
    FOLDER         "test"
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:example_runner>
)

add_subdirectory( examples )

write_tests_file( tests.tsv )
