/**
 * Copyright (c) 2021 Oli Legat
 * Licensed under the BSD 3-Clause License.
 */

// Parses a .cpp.in file an generates a modified .cpp and .out file

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


using ifstream  = std::ifstream;
using ofstream  = std::ofstream;
using string    = std::string;
using stringref = const std::string&;

template <class T>
using vector = std::vector<T>;

const auto npos = std::string::npos;


static string read_file(stringref path)
{
  ifstream in;
  int length;
  string result;

  in.open( path );
  in.seekg(0, std::ios::end);
  length = in.tellg();
  in.seekg(0, std::ios::beg);
  result.resize( length, 0 );
  in.read( &result[0], length );
  in.close();

  return result;
}

static void write_file(stringref path, stringref content)
{
  ofstream out;
  out.open( path );
  out << content;
  out.close();
}

struct sections
{
  string url;
  string source;
  string output;
};

static sections find_sections(stringref text)
{
  const size_t i = text.find("[URL]\n");
  const size_t j = text.find("\n\n[Source]\n");
  const size_t k = text.find("\n\n[Output]\n");
  assert(i != npos);
  assert(j != npos);
  assert(k != npos);

  sections s;
  s.url    = text.substr( i+6,  j-(i+6) );
  s.source = text.substr( j+11, k-(j+10) );
  s.output = text.substr( k+11 );
  return s;
}

static string filename(stringref path)
{
  size_t i = path.rfind("/");
  #if _WIN32
  {
    size_t lastbackslash = path.rfind("\\");
    if (i == npos) {
      i = lastbackslash;
    } else if (lastbackslash == npos) {
      /* skip */
    } else if (lastbackslash > i) {
      i = lastbackslash;
    }
  }
  #endif

  if (i != npos) {
    return path.substr( i+1 );
  } else {
    return string();
  }
}

static string basename(stringref path)
{
  const size_t i = path.rfind(".cpp.in");
  if (i != npos) {
    return path.substr( 0, i );
  } else {
    return path;
  }
}

static string replace_all( stringref str, stringref from, stringref to )
{
  if (from.empty()) { return str; }

  vector<size_t> matches;
  size_t i;

  // Find all matches
  i = 0;
  while(i < str.size()) {
    const char* const a = &str.c_str()[i];
    const char* const b = from.c_str();
    const size_t      n = from.size();
    if (strncmp(a, b, n) == 0) {
      matches.push_back(i);
      i += n;
    }
    else {
      i += 1;
    }
  }

  if (matches.empty()) { return str; }

  // Reserve a string large enough to store the new string
  string result;
  result.resize(str.size()
    - (matches.size() * from.size())
    + (matches.size() * to.size()));

  size_t dstOffset = 0, srcOffset = 0;
  i = 0;
  while (i < matches.size()) {
    // Copy non-matching bytes
    size_t nonMatchingBytes = matches[i] - srcOffset;
    memcpy(&result[dstOffset], &str[srcOffset], nonMatchingBytes);
    dstOffset += nonMatchingBytes;
    srcOffset += nonMatchingBytes;

    // Copy the matching bytes
    memcpy(&result[dstOffset], to.c_str(), to.size());
    dstOffset += to.size();
    srcOffset += from.size();

    i++;
  }

  memcpy(&result[dstOffset], &str[srcOffset], str.size() - srcOffset);

  return result;
}

static void do_file(stringref inpath)
{
  // parse inpath
  const string   txt  = read_file( inpath );
  const string   base = filename( basename( inpath ) );
  const sections sect = find_sections( txt );

  const string patched_source =
    "// Generated from input file : " + inpath + string("\n") +
    "// This example comes from   : " + sect.url + string("\n") +
    "#define COWSTRING_NO_EXPLICIT_STDSTRING 1\n" +
    "#include <cow_string.hpp>\n" +
    replace_all( sect.source, "std::string", "cow::string" );

  write_file( base+".cpp", patched_source );
  write_file( base+".out", sect.output );
}

static void maybe_usage(int argc, char** argv)
{
  string arg1;
  if(argc > 1) arg1 = argv[1];

  if(argc != 2
     || arg1 == "-h"
     || arg1 == "-?"
     || arg1 == "--help"
     || arg1 == "-help")
  {
    printf("usage: test_maker INPUT_FILE.cpp.in\n");
    exit(1);
  }
}

int main(int argc, char** argv)
{
  maybe_usage( argc, argv );
  do_file( argv[1] );
}
