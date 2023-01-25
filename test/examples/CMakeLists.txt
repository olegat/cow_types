add_several_examples(
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
  DEFINES
    -DCOWSTRING_IMPLICIT_STDSTRING_OPERATOR=1
  SOURCES
    basic_string.cpp.in
)

add_several_examples(
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
  SOURCES
    string_append.cpp.in
    string_swap.cpp.in
)

add_several_examples(
  PROPERTIES
    FOLDER         "test/string"
  SOURCES
    string_find.cpp.in
    string_operator_plusequal.cpp.in
)

set( EXAMPLE_TARGETS ${EXAMPLE_TARGETS} PARENT_SCOPE )
