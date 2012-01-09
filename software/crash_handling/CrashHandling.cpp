/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#include <tulip/SystemDefinition.h>
#include <tulip/TulipRelease.h>

#include <iostream>
#include <cstring>

#include "CrashHandling.h"
#include "StackWalker.h"

using namespace std;

/*
  Linux/MacOS-specific handling
 */
#if defined(__linux) || defined(__APPLE__)

#include "UnixSignalInterposer.h"

#if defined(__APPLE__)
#include <sys/ucontext.h>
#else

// This structure mirrors the one found in /usr/include/asm/ucontext.h
typedef struct _sig_ucontext {
  unsigned long     uc_flags;
  struct ucontext   *uc_link;
  stack_t           uc_stack;
  struct sigcontext uc_mcontext;
  sigset_t          uc_sigmask;
} sig_ucontext_t;

#endif

void dumpStack(int sig, siginfo_t *, void * ucontext) {

  // Get the address at the time the signal was raised from the EIP (x86) or RIP (x86_64)

#ifndef __APPLE__

  sig_ucontext_t * uc = reinterpret_cast<sig_ucontext_t *>(ucontext);
#ifdef I64
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.rip); // x86_64 specific;
#else
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.eip); // x86 specific;
#endif

#else

  ucontext_t * uc = reinterpret_cast<ucontext_t *>(ucontext);
#ifndef I64
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext->__ss.__eip);
#else
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext->__ss.__rip);
#endif

#endif

  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
            << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
            << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
            << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl;

  std::cerr << "Caught signal " << sig << " (" << strsignal(sig) << ")" << std::endl;

  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  StackWalkerGCC sw;
  sw.setCallerAddress(callerAddress);
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  installSignalHandler(sig, SIG_DFL);
}

extern void installSignalHandlers(void);

void start_crash_handler() {
  installSignalHandler(SIGSEGV, &dumpStack);
  installSignalHandler(SIGABRT, &dumpStack);
  installSignalHandler(SIGFPE, &dumpStack);
  installSignalHandler(SIGILL, &dumpStack);
  installSignalHandler(SIGBUS, &dumpStack);
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
