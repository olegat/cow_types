/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

#include <string>
#include <vector>

#ifndef PROCESS_INTERNAL_DEFINED
using process_internal = void;
#endif

template <class T>
using vector = std::vector<T>;
using string = std::string;
using int32  = std::int32_t;
using uint64 = std::uint64_t;

struct process
{
  vector<string>     mArgv;
  string             mStdout;
  string             mStderr;
  string             mStdin;
  int32              mExitstatus;
  bool               mTimedOut;

  process_internal*  _internal;

  int start();
  int wait( uint64 timeoutMS = 0 );

  process() :
   mExitstatus(0),
   mTimedOut(false),
   _internal(nullptr)
  {}

  ~process();

  void printDebugInfo();
};
