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

#include "process.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>

using sstream = std::stringstream;

#ifdef _WIN32
#define PROGNAME "example_runner.exe"
#else
#define PROGNAME "example_runner"
#endif

const char* const kUsage =
  "usage: " PROGNAME " [-h|-?|-help|--help]\n"
  "       " PROGNAME " [--] [testsFile1 [testsFile2 ..]]\n"
  "\n"
  "Load & run test files. If " PROGNAME " is run without any arguments, then it\n"
  "will look for a file named \"tests.tsv\" in the current working directory.\n"
  "\n"
  "Each test file must be a TAB-delimited file (.tsv) with 3 columns per line:\n"
  "  Column 1: Full path the test-program binary.\n"
  "  Column 2: Full path to the source code of the test-program.\n"
  "  Column 3: Full path to the expected output of the test-program.\n"
  "\n"
  "Flags:\n"
  "-h,-?,        Show this help message.\n"
  "-help,--help\n"
  "\n"
  "--            stops processing flags. \n"
  "              Example: " PROGNAME " -- -h reads file \"-h\"\n";

static bool is_help( const char* cstr ) {
  string s = cstr;
  return s == "-h" || s == "-?" || s == "-help" || s == "--help";
}

static void parse_args( int* argc, const char*** argv )
{
  static const char* defaultArgv[] { "tests.tsv", nullptr };
  if( *argc == 1 ) {
    *argv = defaultArgv;
    return;
  } else if( is_help( (*argv)[1] )) {
    std::cout << kUsage;
    std::exit(0);
  } else if( strcmp((*argv)[1], "--") == 0 ) {
    *argc = (*argc) - 2;
    *argv = &(*argv)[2];
  } else {
    *argc = (*argc) - 1;
    *argv = &(*argv)[1];
  }
}

void run_test_file( const string& path)
{
#if _DEBUG
  const uint64 kTimeoutMS = 0; // no-timeout
#else
  const uint64 kTimeoutMS = 1000;
#endif
  const tsv table = read_tsv( path );

  for( const tsv_line& line : table.lines ) {
    process p;
    p.mArgv = { line.binary_path };
    int sr = p.start();
    int wr = p.wait(kTimeoutMS);

    string expectedOutput = read_text(line.output_path);

    sstream errMsg;
    if( p.mTimedOut != 0 ) {
      errMsg << "Program timed-out (" << wr << "ms)";
    } else if( p.mExitstatus != 0 ) {
      errMsg << "Program exited with status " << p.mExitstatus;
    } else if ( p.mStdout != expectedOutput ) {
      errMsg << "Program output is incorrect";
    }

    string msg = errMsg.str();
    string hr (50, '-');
    std::cout << hr << std::endl;
    if( ! msg.empty() ) {
      std::cout << "Test " << p.mArgv[0] << " FAILED!" << std::endl;
      std::cout << "Reason: " << msg << std::endl;

      struct{ const char* name; const string& content; } iostreams[] {
        { "Expected Output:", expectedOutput },
        { "stdout:", p.mStdout},
        { "stderr:", p.mStderr},
        { "stdin:" , p.mStdin },
      };
      for( auto io : iostreams ) {
        std::cout << std::endl << io.name << std::endl
          << io.content << "(EOF)" << std::endl;
      }
    } else {
      std::cout << "Test " << p.mArgv[0] << " PASSED!" << std::endl;
    }
  }
}

int main(int argc, const char** argv)
{
  parse_args( &argc, &argv );

  for( int i = 0; i < argc; i++ ) {
    run_test_file( string(argv[i]) );
  }
}
