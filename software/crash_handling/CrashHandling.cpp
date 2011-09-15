/*
 * CrashHandling.cpp
 *
 *  Created on: Aug 14, 2011
 *      Author: antoine
 */

#include "CrashHandling.h"
#include "StackWalker.h"

#include <tulip/SystemDefinition.h>
#include <tulip/TulipRelease.h>

/*
  Linux/MacOS-specific handling
  */
#if defined(__linux) || defined(__APPLE__)

#include <iostream>
#include <signal.h>

static void dumpStack(int signum) {
  StackWalkerGCC sw;
  std::cerr << "Caught signal " << (int)signum << std::endl;

  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
  << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
  << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
  << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl;

  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  signal(signum, SIG_DFL);
}

void start_crash_handler() {
  signal(SIGSEGV,dumpStack);
  signal(SIGABRT,dumpStack);
  signal(SIGFPE,dumpStack);
  signal(SIGILL,dumpStack);
  signal(SIGBUS,dumpStack);
  signal(-1,dumpStack);
}

/*
  MinGW-specific handling
  */
#elif defined(__MINGW32__)

static LONG WINAPI
exception_filter(LPEXCEPTION_POINTERS info) {
  StackWalkerMinGW sw;
  sw.setContext(info->ContextRecord);

  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
  << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
  << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
  << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl;
  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  return 1;
}


void start_crash_handler() {
  SetUnhandledExceptionFilter(exception_filter);
}

#elif defined(_MSC_VER)

static LONG WINAPI
exception_filter(LPEXCEPTION_POINTERS info) {
  StackWalkerMSVC sw;
  sw.setContext(info->ContextRecord);

  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
  << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
  << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
  << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl;
  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  return 1;
}


void start_crash_handler() {
  SetUnhandledExceptionFilter(exception_filter);
}
#endif
