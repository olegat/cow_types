# Copyright (c) 2021 Oli Legat
# Licensed under the BSD 3-Clause License.
#

define_property( TARGET
  PROPERTY   EXAMPLE_SOURCE
  BRIEF_DOCS "The generated .cpp file for this test example."
  FULL_DOCS  "This property is added to targets created using add_example.
It is the source code of the example."
)

define_property( TARGET
  PROPERTY   EXAMPLE_OUTPUT
  BRIEF_DOCS "The generated .out file for this test example."
  FULL_DOCS  "This property is added to targets created using add_example.
It is the expected output of the example."
)

function(add_target tgt)
  cmake_parse_arguments(args
    ""
    "TYPE"
    "SOURCES;SOURCES_UNIX;SOURCES_WIN32;PROPERTIES"
    ${ARGN} )
  # Add & config target:
  if(DEFINED args_SOURCES)
    if( ${args_TYPE} STREQUAL "executable" )
      add_executable( ${tgt} ${args_SOURCES} )
    elseif( ${args_TYPE} STREQUAL "library" )
      add_library( ${tgt} ${args_SOURCES} )
    else()
      message(FATAL_ERROR "Unknown target TYPE: ${TYPE}")
    endif()
  endif()
  # Add platform-specific sources:
  if(UNIX AND DEFINED args_SOURCES_UNIX)
    target_sources( ${tgt} PRIVATE ${args_SOURCES_UNIX} )
  elseif(WIN32 AND DEFINED args_SOURCES_WIN32)
    target_sources( ${tgt} PRIVATE ${args_SOURCES_WIN32} )
  endif()
  # Add target properties:
  if(DEFINED args_PROPERTIES)
    set_target_properties(${tgt} PROPERTIES ${args_PROPERTIES})
  endif()
endfunction()

function(add_example tgt)
  cmake_parse_arguments(args "" "" "SOURCES;PROPERTIES;DEFINES" ${ARGN} )
  # Add & config target:
  set(EXAMPLE_TARGETS ${EXAMPLE_TARGETS} ${tgt} PARENT_SCOPE)
  add_executable( ${tgt} )
  if(NOT TARGET example_runner)
    message(FATAL_ERROR "Cannot call add_example before add target \"example_runner\"")
  endif()
  add_dependencies( example_runner ${tgt} )
  target_link_libraries( ${tgt} PRIVATE cow_string )
  if(DEFINED args_PROPERTIES)
    set_target_properties(${tgt} PROPERTIES ${args_PROPERTIES})
  endif()
  if(DEFINED args_DEFINES)
    target_compile_definitions(${tgt} PRIVATE ${args_DEFINES})
  endif()
  # Add custom commands for "*.cpp.in" files:
  set(have_cpp_in FALSE)
  foreach(src "${args_SOURCES}" )
    get_filename_component( src  ${src} ABSOLUTE )
    get_filename_component( ext  ${src} EXT )
    get_filename_component( base ${src} NAME_WE )
    if("${ext}" MATCHES "^.cpp.in$")
      if( have_cpp_in )
        message(FATAL_ERROR "Can't add ${src} for target ${tgt} because another .cpp.in file was already added (only 1 .cpp.in file allowed).")
      endif()
      set( have_cpp_in TRUE )
      add_custom_command(
        MAIN_DEPENDENCY
          ${src}
        DEPENDS
          example_compiler
        OUTPUT
          ${CMAKE_CURRENT_BINARY_DIR}/${base}.cpp
          ${CMAKE_CURRENT_BINARY_DIR}/${base}.out
        WORKING_DIRECTORY
          ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND
          $<TARGET_FILE:example_compiler> ${src}
      )
      target_sources( ${tgt}
        PRIVATE
          ${src}
          ${CMAKE_CURRENT_BINARY_DIR}/${base}.cpp
      )
      set_target_properties( ${tgt}
        PROPERTIES
          EXAMPLE_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${base}.cpp
          EXAMPLE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${base}.out
      )
    else()
      target_sources( ${tgt} PRIVATE ${src} )
    endif()
  endforeach()
endfunction()

function(write_tests_file filename)
  set(content)
  foreach(tgt ${EXAMPLE_TARGETS})
    # Get properties and check.
    get_target_property( src ${tgt} EXAMPLE_SOURCE )
    get_target_property( out ${tgt} EXAMPLE_OUTPUT )
    if( NOT src )
      message( FATAL_ERROR "Property EXAMPLE_SOURCE not found for target ${tgt}")
    endif()
    if( NOT out )
      message( FATAL_ERROR "Property EXAMPLE_OUTPUT not found for target ${tgt}")
    endif()
    # Append TAB-delimited line
    set(content
      "${content}$<TARGET_FILE:${tgt}>\t${src}\t${out}\n")
  endforeach()
  file(GENERATE
    OUTPUT  $<CONFIG>/${filename}
    CONTENT ${content}
  )
endfunction()
