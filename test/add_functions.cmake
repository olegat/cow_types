# Copyright (c) 2021 Oli Legat
# Licensed under the BSD 3-Clause License.
#

function(add_compiler tgt)
  cmake_parse_arguments(args "" "" "SOURCES;PROPERTIES" ${ARGN} )
  # Add & config target:
  if(DEFINED args_SOURCES)
    add_executable( ${tgt} ${args_SOURCES})
  endif()
  if(DEFINED args_PROPERTIES)
    set_target_properties(${tgt} PROPERTIES ${args_PROPERTIES})
  endif()
endfunction()

function(add_cpp_in tgt)
  cmake_parse_arguments(args "" "" "SOURCES;PROPERTIES" ${ARGN} )
  # Add & config target:
  add_executable( ${tgt} )
  target_link_libraries( ${tgt} PRIVATE cow_string )
  if(DEFINED args_PROPERTIES)
    set_target_properties(${tgt} PROPERTIES ${args_PROPERTIES})
  endif()
  # Add custom commands for "*.cpp.in" files:
  foreach(src "${args_SOURCES}" )
    get_filename_component( src  ${src} ABSOLUTE )
    get_filename_component( ext  ${src} EXT )
    get_filename_component( base ${src} NAME_WE )
    if("${ext}" MATCHES "^.cpp.in$")
      add_custom_command(
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
    else()
      target_sources( ${tgt} PRIVATE ${src} )
    endif()
  endforeach()
  # Config target
endfunction()
