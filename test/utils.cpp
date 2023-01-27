/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include "utils.hpp"
#include <stdio.h>
#include <sstream>

using sstream = std::stringstream;

vector<string> split_string( const string& str, const string& delim )
{
  vector<string> result;
  size_t start = 0;
  size_t end   = 0;
  while( (end = str.find(delim, start)) != string::npos ) {
    result.push_back( str.substr (start, end - start) );
    start = end + delim.length();
  }
  if( start != str.length() - 1) {
    result.push_back( str.substr (start) );
  }
  return result;
}

vector<string> slice_lines( const string& str )
{
  vector<string> result;
  size_t start = 0;
  size_t end   = 0;
  auto add_substr = [&result, &str](size_t start, size_t end) {
    string s = str.substr(start, end - start);
    if ( ! s.empty() ) {
      result.push_back(s);
    }
  };
  while( end < str.size() ) {
    if (str[end] == '\r' || str[end] == '\n' || str[end] == '\t' || str[end] == '\0') {
      add_substr(start, end);
      add_substr(end, end+1);
      start = end + 1;
    }
    end++;
  }
  add_substr(start, end);
  return result;
}

vector<string> split_lines(const string& str)
{
  // Guess EOL mode (CRLF Windows/DOS, LF macOS/Unix)
  string delim = "\n";
  if (str.find("\r\n") != string::npos) {
    delim = "\r\n";
  }
  return split_string(str, delim);
}

string read_text( const string& filepath )
{
  const size_t bufferSize = 1024;
  char buffer[bufferSize] = {0};
  sstream ss;
  size_t n;

  FILE* file = fopen(filepath.c_str(), "rb");
  if( file != NULL ) {
    do {
      n = fread( buffer, 1, bufferSize, file );
      ss.write( buffer, n );
    } while( n == bufferSize );
    fclose( file );
  }

  return ss.str();
}

tsv read_tsv( const string& filepath )
{
  string text = read_text( filepath );

  vector<string> lines = split_lines( text );

  tsv result;
  for( string line : lines ) {
    if( line.empty() ) {
      continue;
    }
    vector<string> columns = split_string( line, "\t" );
    tsv_line row;
    if( columns.size() >= 1 ) {
      row.binary_path = columns[0];
    }
    if( columns.size() >= 2 ) {
      row.source_path = columns[1];
    }
    if( columns.size() >= 3 ) {
      row.output_path = columns[2];
    }
    result.lines.push_back( row );
  }

  return result;
}
