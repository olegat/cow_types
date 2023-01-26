/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

// Based on MSDN sample:
//   https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
//
using process_internal = struct process_win32;
#define PROCESS_INTERNAL_DEFINED 1
#include "process.hpp"

#include <windows.h>

#include <iostream>
#include <cassert>

struct process_win32
{
  struct pipe {
    HANDLE readable;
    HANDLE writable;
  };

  pipe                stdOut;
  pipe                stdErr;
  pipe                stdIn;
  PROCESS_INFORMATION procinfo;

  ~process_win32() {
  }
};

static BOOL create_pipe(
  const char* debug_name,
  process_win32::pipe& pipe,
  HANDLE* parent_end)
{
  assert( parent_end == &pipe.readable || parent_end == &pipe.writable );

  SECURITY_ATTRIBUTES attr = {
    /* .nLength =*/ sizeof(SECURITY_ATTRIBUTES),
    /* .lpSecurityDescriptor = */ NULL,
    /* .bInheritHandle = */ TRUE
  };

  if( ! CreatePipe(&pipe.readable, &pipe.writable, &attr, 0)) {
    std::cerr << "CreatePipe error: " << debug_name << std::endl;
    return FALSE;
  }

  // Don't allow the child to inherit the end that is owned by the parent
  if( ! SetHandleInformation(*parent_end, HANDLE_FLAG_INHERIT, 0)) {
    std::cerr << "SetHandleInformation error: " << debug_name << std::endl;
    return FALSE;
  }

  return TRUE;
}

static BOOL create_all_pipes(process_win32* p)
{
  assert( p != nullptr );
  return
    create_pipe("stdout", p->stdOut, &p->stdOut.readable) &&
    create_pipe("stderr", p->stdErr, &p->stdErr.readable) &&
    create_pipe("stdin",  p->stdIn,  &p->stdIn .writable) ;
}

static void close_handle(HANDLE* h)
{
  assert( h != nullptr );
  CloseHandle(*h);
  *h = NULL;
}

static void close_all_handles(process_win32* p)
{
  assert( p != nullptr );
  HANDLE* all_pipes[] = {
    &p->stdOut.readable,
    &p->stdOut.writable,
    &p->stdErr.readable,
    &p->stdErr.writable,
    &p->stdIn.readable,
    &p->stdIn.writable,
    &p->procinfo.hProcess,
    &p->procinfo.hThread
  };
  for (HANDLE* h : all_pipes) {
    close_handle(h);
  }
}

static BOOL start_child( process* p )
{
  STARTUPINFO startinfo = {0};
  startinfo.cb = sizeof(STARTUPINFO);
  startinfo.hStdOutput = p->_internal->stdOut.writable;
  startinfo.hStdError  = p->_internal->stdErr.writable;
  startinfo.hStdInput  = p->_internal->stdIn.readable;
  startinfo.dwFlags |= STARTF_USESTDHANDLES;

  // TODO: pass args (p->mArgv[1:]) to command-line
  char* cmdline = new char[p->mArgv[0].size() + 1];
  std::memcpy(cmdline, p->mArgv[0].c_str(), p->mArgv[0].size());
  cmdline[p->mArgv[0].size()] = '\0';

  BOOL success = CreateProcessA(
    NULL,
    cmdline,     // TODO: convert to UTF16 and use CreateProcessW
    NULL,        // security attributes
    NULL,        // thread security attributes
    TRUE,        // handles are inherited
    0,           // creation flags
    NULL,        // use parent's environment
    NULL,        // use parent's working directory
    &startinfo,
    &p->_internal->procinfo);

  delete[] cmdline;

  if( ! success ) {
    std::cerr << "CreateProcess failed: " << p->mArgv[0] << std::endl;
  } else {
    close_handle(&p->_internal->stdOut.writable);
    close_handle(&p->_internal->stdErr.writable);
    close_handle(&p->_internal->stdIn.readable);
  }

  return success;
}

static string read_pipe(const process_win32::pipe& p) {
   BOOL success = TRUE;
   DWORD size = 1;
   CHAR buffer[1024] = {0};
   string result;

   while (success && size > 0) {
     success = ReadFile(p.readable, buffer, sizeof(buffer), &size, NULL);
     result.append(buffer, size);
   }

   result.shrink_to_fit();
   return result;
}

process::~process()
{
  delete _internal;
}

int process::start()
{
  assert( _internal == nullptr );
  _internal = new process_win32();
  ZeroMemory(_internal, sizeof(process_win32));

  if (!create_all_pipes(_internal)) {
    std::cerr << "Failed to create all pipes" << std::endl;
    return -1;
  }
  if (!start_child(this)) {
    std::cerr << "Failed to start child" << std::endl;
    return -1;
  }
  return 0;
}

int process::wait( uint64 timeoutMS )
{
  // Wait for process to finish (or timeout & terminate)
  HANDLE hproc = _internal->procinfo.hProcess;
  DWORD dwTimeout = timeoutMS == 0 ? INFINITE : static_cast<DWORD>(timeoutMS);
  DWORD status = WaitForSingleObject(hproc, dwTimeout);
  if (status == WAIT_TIMEOUT) {
    this->mTimedOut = true;
  }
  if (status != WAIT_OBJECT_0) {
    TerminateProcess(hproc, -1);
  }

  // Get a exit code of the process
  DWORD exit_code = -1;
  BOOL success = GetExitCodeProcess(hproc, &exit_code);
  if (!success) {
    std::cerr << "GetExitCodeProcess failed" << std::endl;
  }
  this->mExitstatus = static_cast<int32>(exit_code);

  if (! this->mTimedOut ) {
    this->mStdout = read_pipe(_internal->stdOut);
    this->mStderr = read_pipe(_internal->stdErr);
  }
  close_all_handles(_internal);
  return -1;
}

void process::printDebugInfo()
{
  // TODO
}
