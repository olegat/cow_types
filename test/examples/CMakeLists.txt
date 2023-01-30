# Note: The end examples (`end`, `rend`, `cend`, `crend`) from cplusplus.com are
# excluded because they are identical to their begin counter-parts

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
    string_erase.cpp.in
    string_insert.cpp.in
    string_swap.cpp.in
)

add_several_examples(
  PROPERTIES
    CXX_STANDARD   11
    CXX_EXTENSIONS OFF
    FOLDER         "test/string"
  SOURCES
    string_back.cpp.in
    string_cbegin.cpp.in
    string_crbegin.cpp.in
    string_front.cpp.in
    # string_operator_plus.cpp.in
    string_pop_back.cpp.in
    # string_shrink_to_fit.cpp.in
)

add_several_examples(
  PROPERTIES
    FOLDER         "test/string"
  SOURCES
    # string_assign.cpp.in
    # string_at.cpp.in
    string_begin.cpp.in
    string_c_str.cpp.in
    # string_capacity.cpp.in
    # string_compare.cpp.in
    # string_copy.cpp.in
    string_data.cpp.in
    string_find.cpp.in
    # string_find_first_not_of.cpp.in
    # string_find_first_of.cpp.in
    # string_find_last_not_of.cpp.in
    # string_find_last_of.cpp.in
    string_length.cpp.in
    # string_max_size.cpp.in
    # string_operator_equal.cpp.in
    string_operator_plusequal.cpp.in
    string_operator_squarebrackets.cpp.in
    # string_operators.cpp.in
    string_rbegin.cpp.in
    string_replace.cpp.in
    # string_resize.cpp.in
    string_rfind.cpp.in
    string_size.cpp.in
    string_string.cpp.in
    string_substr.cpp.in
    string_swap-free.cpp.in
)

set( EXAMPLE_TARGETS ${EXAMPLE_TARGETS} PARENT_SCOPE )
