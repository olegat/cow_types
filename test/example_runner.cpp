/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include "process.hpp"
#include "utils.hpp"
#include "term.hpp"
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

  const string hr (50, '-');
  string hr0;
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
    if( ! msg.empty() ) {
      if ( hr0.empty() ) {
        term::out << hr << term::endl;
        hr0 = hr;
      }
      term::out
        << term::redfg
        << "Failed: " << p.mArgv[0] << term::reset << term::endl
        << "Reason: " << msg << term::endl;

      struct{ const char* name; const string& content; } iostreams[] {
        { "Expected Output:", expectedOutput },
        { "stdout:", p.mStdout},
        { "stderr:", p.mStderr},
        { "stdin:" , p.mStdin },
      };
      for( auto io : iostreams ) {
        term::out
          << term::endl << io.name << term::endl;

        // Draw common undisplayed characters like \r, \n or \t
        vector<string> slices = slice_lines(io.content);
        for (const string& slice : slices) {
          if (slice == "\t") {
            term::out << term::grayfg << "\\t" << term::reset;
          } else if (slice[0] == '\0') {
            term::out << term::grayfg << "\\0" << term::reset;
          } else if (slice == "\r") {
            term::out << term::grayfg << "\\r" << term::reset;
          } else if (slice == "\n") {
            term::out << term::grayfg << "\\n" << term::reset << term::endl;
          } else {
            term::out << slice;
          }
        }
        term::out << term::grayfg << "(EOF)" << term::reset << term::endl;
      }
      term::out << hr << term::endl;
    } else {
      term::out
        << term::greenfg << "Passed: " << p.mArgv[0]
        << term::reset << term::endl;
      hr0.clear();
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
