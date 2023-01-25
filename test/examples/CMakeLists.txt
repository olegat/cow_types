add_example( basic_string
  SOURCES
    basic_string.cpp.in
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
  DEFINES
    -DCOWSTRING_IMPLICIT_STDSTRING_OPERATOR=1
)

add_example( string_append
  SOURCES
    string_append.cpp.in
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
)

add_example( string_swap
  SOURCES
    string_swap.cpp.in
  PROPERTIES
    CXX_STANDARD   14
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
)

add_example( string_find
  SOURCES
    string_find.cpp.in
  PROPERTIES
    FOLDER         "test/string"
)

add_example( string_operator_plusequal
  SOURCES
    string_operator_plusequal.cpp.in
  PROPERTIES
    FOLDER         "test/string"
)

set( EXAMPLE_TARGETS ${EXAMPLE_TARGETS} PARENT_SCOPE )
