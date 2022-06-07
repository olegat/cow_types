/**
 * Copyright (c) 2021 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
