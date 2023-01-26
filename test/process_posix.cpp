/**
 * Copyright (c) 2023 Oli Legat <http://github.com/olegat>.
 * Licensed under the BSD 3-Clause License.
 */

using process_internal = struct process_posix;
#define PROCESS_INTERNAL_DEFINED 1
#include "process.hpp"

#include <unistd.h>
#include <iostream>
#include <sstream>

using sstream = std::stringstream;

struct process_posix
{
  union {
    int      all[6];
    struct {
    int      out[2];
    int      err[2];
    int      in [2];
    } std;
  } pipes;

  pid_t  pid;
};

#define    INIT_PIPE( _pipevar ) init_pipe( _pipevar, #_pipevar )
static int init_pipe( int (&fd)[2], const char* debug_name )
{
  assert( debug_name != nullptr );
  int err = pipe(fd);
  if( err == -1 ) {
    std::cerr << "error initializing: " << debug_name << std::endl;
  }
  return(err);
}

static void close_all_pipes( process_posix* pp )
{
  assert( pp != nullptr );
  for( int fd : pp->pipes.all ) {
    if( fd != 0 ) {
      close(fd);
    }
  }
  bzero( &pp->pipes.all[0], sizeof(pp->pipes.all) );
}

static char** alloc_argv_array( const vector<string>& inArgv)
{
  // Calculate sizes in bytes
  const size_t argc        = inArgv.size();
  const size_t tableSize   = (argc + 1) * sizeof(char*);
  size_t       contentSize = 0;
  for( const string& a : inArgv ) {
    contentSize += a.size() + 1;
  }

  // Allocate
  char*  const start = (char*) calloc( 1, tableSize + contentSize );
  char** const table = reinterpret_cast<char**>( start );

  // Copy strings
  size_t offset = tableSize;
  for( size_t i = 0; i < argc; i++ )
  {
          char* const dst   = &start[offset];
    const char* const src   = inArgv[i].c_str();
    const size_t      bytes = inArgv[i].size() + 1;
    memcpy( dst, src, bytes );
    table[i] = dst;
    offset += bytes;
  }

  return table;
}

static void start_child( process* pp )
{
#if 0
  // Debugging aid:
  int wait=1; while(wait) {sleep(10);}
#endif
  // redirect std* pipes to shared child/parent pipes.
  auto pipes = pp->_internal->pipes;
  int r = dup2( pipes.std.out[1], STDOUT_FILENO );
  int s = dup2( pipes.std.err[1], STDERR_FILENO );
  int t = dup2( pipes.std.in[0], STDIN_FILENO );
  close_all_pipes( pp->_internal );

  const char*  prog = pp->mArgv[0].c_str();
  char**       argv = alloc_argv_array( pp->mArgv );
  if( execv( prog, argv ) != 0 ) {
    std::cerr << "error starting program: " << prog << std::endl;
    std::exit(1);
  }
}

static void start_parent( process* pp )
{
  // close unused pipes.
  auto pipes = pp->_internal->pipes;
  close( pipes.std.out[1] );
  close( pipes.std.err[1] );
  close( pipes.std.in[0] );

  // send stdin data.
  const string& input = pp->mStdin;
  write( pipes.std.in[1], input.c_str(), input.size() );
}

static uint64_t time_ms()
{
  uint64_t        ms; // Milliseconds
  uint64_t        s;  // Seconds
  struct timespec spec;

  clock_gettime(CLOCK_REALTIME, &spec);
  s  = spec.tv_sec;
  ms = spec.tv_nsec / 1.0e6;
  return (s * 1000) + ms;
}

static string read_pipe(int fd)
{
  sstream result;
  char buffer[1024] = {0};
  ssize_t n;
  do {
    n = read( fd, buffer, sizeof(buffer) );
    if( n > 0 ) {
      result.write( buffer, n );
    }
  } while(n > 0);

  return result.str();
}

process::~process()
{
  free( _internal );
}

int process::start()
{
  assert( _internal == nullptr );
  _internal = (process_internal*) calloc( 1, sizeof(process_posix) );

  int err = 0;

  ( err = INIT_PIPE(_internal->pipes.std.out) ) ||
  ( err = INIT_PIPE(_internal->pipes.std.err) ) ||
  ( err = INIT_PIPE(_internal->pipes.std.in)  ) ;

  if( err != 0 ) {
    close_all_pipes( _internal );
  } else {
    _internal->pid = fork();
    if( _internal->pid == 0 ) {
      start_child( this );
    } else {
      start_parent( this );
    }
  }
  return(err);
}

int process::wait( uint64 timeoutMS )
{
  uint64_t const sleepMS = 10;
  uint64_t       timer   = 0;
  uint64_t       now     = time_ms();

  int   status = 0;
  pid_t pid    = 0;
  while( pid == 0 && timer < timeoutMS )
  {
    const uint64_t delta = time_ms() - now;
    timer += delta;
    now   += delta;

    pid = waitpid( _internal->pid, &status, WNOHANG );
  }

  // Results:
  this->mTimedOut = (timer >= timeoutMS);
  if( !this->mTimedOut )
  {
    this->mExitstatus = status;
    // TODO(olegat@) read some bytes of stdout/err in while loop above
    this->mStdout = read_pipe( _internal->pipes.std.out[0] );
    this->mStderr = read_pipe( _internal->pipes.std.err[0] );
  }
  return status;
}

void process::printDebugInfo()
{
  const string hr = "--------------------------------------------------\n";
  sstream ss;
  ss << hr << "Command Line : ";
  for( const string& a : mArgv ) {
    // TODO(olegat@) add "" for args with spaces and escape ".
    ss << " " << a;
  }
  ss << "\n";

  ss << "Exit status  :  ";
  if( mTimedOut ) {
    ss << "N/A (Timed Out)";
  } else {
    ss << mExitstatus;
  }
  ss << "\n";

  ss << hr << "stdout :\n" << mStdout;
  ss << hr << "stderr :\n" << mStderr;
  ss << hr << "stdin  :\n" << mStdin << hr;

  std::cout << ss.str();
}
