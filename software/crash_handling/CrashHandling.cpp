/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include <tulip/SystemDefinition.h>
#include <tulip/TulipRelease.h>

#include <iostream>
#include <fstream>
#include <cstring>

#include "CrashHandling.h"
#include "StackWalker.h"

using namespace std;

static std::string TULIP_DUMP_FILE = "";

void CrashHandling::setDumpPath(string s) {
  TULIP_DUMP_FILE = s;
}

#ifdef _MSC_VER

static std::string SYMBOLS_SEARCH_PATHS = "";

void CrashHandling::setExtraSymbolsSearchPaths(const std::string &searchPaths) {
  SYMBOLS_SEARCH_PATHS = searchPaths;
}

#endif

static void dumpStackTrace(StackWalker &sw) {
  std::ostream *os = &std::cerr;
  std::ofstream ofs;

  if (!TULIP_DUMP_FILE.empty()) {
    ofs.open(TULIP_DUMP_FILE.c_str());

    if (!ofs.is_open()) {
      std::cerr << "Could not open " << TULIP_DUMP_FILE << std::endl;
    }
    else {
      os = &ofs;
      std::cerr << "Writing dump stack to " << TULIP_DUMP_FILE << std::endl;
    }
  }

  *os << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
      << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
      << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
      << TLP_VERSION_HEADER << " " << TULIP_VERSION  << std::endl;
  *os << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStack(*os, 50);
  *os << TLP_STACK_END_HEADER << std::endl;
  *os << std::flush;

  if (ofs.is_open()) {
    ofs.close();
  }
}

/*
  Linux/MacOS-specific handling
 */
#if defined(__unix__) || defined(__APPLE__)

#if defined(__i386__) || defined(__amd64__)

#include "UnixSignalInterposer.h"

#if defined(__APPLE__)
#include <sys/ucontext.h>
#elif defined(__FreeBSD__)
#include <sys/ucontext.h>
typedef ucontext_t sig_ucontext_t;
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
#if defined(__i386__)
#ifdef __FreeBSD__
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.mc_eip); // x86 specific;
#else
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.eip); // x86 specific;
#endif
#else
#ifdef __FreeBSD__
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.mc_rip); // x86_64 specific;
#else
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext.rip); // x86_64 specific;
#endif
#endif

#else

  ucontext_t * uc = reinterpret_cast<ucontext_t *>(ucontext);
#if defined(__i386__)
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext->__ss.__eip);
#else
  void *callerAddress = reinterpret_cast<void *>(uc->uc_mcontext->__ss.__rip);
#endif

#endif

  StackWalkerGCC sw;
  sw.setCallerAddress(callerAddress);

  dumpStackTrace(sw);

  installSignalHandler(sig, SIG_DFL);
}

extern void installSignalHandlers(void);

void CrashHandling::installCrashHandler() {
  installSignalHandler(SIGSEGV, &dumpStack);
  installSignalHandler(SIGABRT, &dumpStack);
  installSignalHandler(SIGFPE, &dumpStack);
  installSignalHandler(SIGILL, &dumpStack);
  installSignalHandler(SIGBUS, &dumpStack);
}

#else
// architecture not supported
void CrashHandling::installCrashHandler() {}
#endif

/*
  MinGW-specific handling
 */
#elif defined(__MINGW32__)

static LONG WINAPI
exception_filter(LPEXCEPTION_POINTERS info) {
  StackWalkerMinGW sw;
  sw.setContext(info->ContextRecord);

  dumpStackTrace(sw);

  return 1;
}

void CrashHandling::installCrashHandler() {
  SetUnhandledExceptionFilter(exception_filter);
}

#elif defined(_MSC_VER)

static LONG WINAPI
exception_filter(LPEXCEPTION_POINTERS info) {
  StackWalkerMSVC sw;
  sw.setExtraSymbolsSearchPaths(SYMBOLS_SEARCH_PATHS);
  sw.setContext(info->ContextRecord);

  dumpStackTrace(sw);

  return 1;
}


void CrashHandling::installCrashHandler() {
  SetUnhandledExceptionFilter(exception_filter);
}
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
