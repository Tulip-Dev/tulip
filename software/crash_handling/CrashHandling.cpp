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
  Linux-specific handling
  */
#if defined(__linux)

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

#include <dbghelp.h>
#include <client/windows/handler/exception_handler.h>

static std::wstring ctow(const char *cs) {
  std::string s(cs);
  wchar_t *buf = new wchar_t[ s.size() ];
  size_t num_chars = mbstowcs( buf, s.c_str(), s.size() );
  std::wstring ws( buf, num_chars );
  delete[] buf;
  return ws;
}

static std::string wtos(const wchar_t *cs) {
  std::wstring s(cs);
  char *buf = new char[ s.size() ];
  size_t num_chars = wcstombs(buf, s.c_str(), s.size() );
  std::string ret( buf, num_chars );
  delete[] buf;
  return ret;
}

static bool dumpCallback(const wchar_t* dump_path,
                         const wchar_t* minidump_id,
                         void* context,
                         EXCEPTION_POINTERS* exinfo,
                         MDRawAssertionInfo* assertion,
                         bool succeeded) {
  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
            << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
            << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
            << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl
            << TLP_DUMP_HEADER << " " << wtos(dump_path) << "/" << wtos(minidump_id) << ".dmp" << std::endl;

  StackWalkerMSVC sw;
  sw.setContext(exinfo->ContextRecord);
  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  return succeeded;
}

void start_crash_handler() {
  char *tmpDir = getenv("TEMP");
  new google_breakpad::ExceptionHandler(ctow(tmpDir), NULL, dumpCallback, NULL, google_breakpad::ExceptionHandler::HANDLER_EXCEPTION);

}
#endif
