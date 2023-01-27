/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include <string>
#include <vector>

template <class T>
using vector = std::vector<T>;
using string = std::string;

struct tsv_line {
  string binary_path;
  string source_path;
  string output_path;
};
struct tsv {
  string path;
  vector< tsv_line > lines;
};

// Split a string
vector<string> split_string( const string& str, const string& delim );

// Seperate out \n \r \t \0 into substrings
vector<string> slice_lines( const string& str );

// Read a text file.
string read_text( const string& filepath );

// Reading "tests.tsv"
tsv read_tsv( const string& filepath );
