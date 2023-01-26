# Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
# Licensed under the BSD 3-Clause License.
#
cmake_minimum_required(VERSION 3.12)
project(cow_types)

option(COW_STRING_ENABLE_TESTS "Add CMake subdirectory 'test/'" ON)

add_library(cow_string INTERFACE)

target_include_directories(cow_string INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(COW_STRING_ENABLE_TESTS)
  add_subdirectory(test)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT "example_runner")
endif()
