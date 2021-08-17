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
