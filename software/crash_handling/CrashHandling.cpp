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

#if defined(__linux)
#include <iostream>
#include <signal.h>
#include <cstring>
#include <client/linux/handler/exception_handler.h>

typedef struct _sig_ucontext {
  unsigned long     uc_flags;
  struct ucontext   *uc_link;
  stack_t           uc_stack;
  struct sigcontext uc_mcontext;
  sigset_t          uc_sigmask;
} sig_ucontext_t;


typedef struct _crash_infos {
  int signum;
  void *fault_addr;
  void * caller_address;
} crash_infos;

static bool handlerCallback(const void* crash_context,
                            size_t,
                            void* context) {

  const google_breakpad::ExceptionHandler::CrashContext *crashContext = reinterpret_cast<const google_breakpad::ExceptionHandler::CrashContext*>(crash_context);
  const sig_ucontext_t * uc = reinterpret_cast<const sig_ucontext_t *>(&(crashContext->context));
  const siginfo_t *siginfo = reinterpret_cast<const siginfo_t *>(&(crashContext->siginfo));

  crash_infos *ci = reinterpret_cast<crash_infos*>(context);
  ci->signum = siginfo->si_signo;
  ci->fault_addr = siginfo->si_addr;

#ifdef I64
  ci->caller_address = reinterpret_cast<void *>(uc->uc_mcontext.rip); // x86_64 specific;
#else
  ci->caller_address = reinterpret_cast<void *>(uc->uc_mcontext.eip); // x86 specific;
#endif

  return false;
}

static bool dumpCallback(const char* dump_path, const char* minidump_id, void* context, bool succeeded) {
  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
            << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
            << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
            << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl
            << TLP_DUMP_HEADER << " " << dump_path << "/" << minidump_id << ".dmp" << std::endl;

  crash_infos *ci = reinterpret_cast<crash_infos*>(context);

  std::cerr << "signal " << ci->signum
            << " (" << strsignal(ci->signum) << "), address is "
            << ci->fault_addr << " from " << ci->caller_address
            << std::endl;

  StackWalkerGCC sw;
  sw.setCallerAddress(ci->caller_address);
  std::cerr << TLP_STACK_BEGIN_HEADER << std::endl;
  sw.printCallStackToStdErr();
  std::cerr << TLP_STACK_END_HEADER << std::endl;
  std::cerr << std::flush;
  return succeeded;
}

void start_crash_handler() {
  google_breakpad::ExceptionHandler *exceptionHandler = new google_breakpad::ExceptionHandler("/tmp", NULL, dumpCallback, new crash_infos(), true);
  exceptionHandler->set_crash_handler(handlerCallback);
}

#elif defined(__MINGW32__)



static LONG WINAPI
exception_filter(LPEXCEPTION_POINTERS info) {

  std::cerr << TLP_PLATEFORM_HEADER << " " << OS_PLATFORM << std::endl
            << TLP_ARCH_HEADER << " "  << OS_ARCHITECTURE << std::endl
            << TLP_COMPILER_HEADER << " "  << OS_COMPILER  << std::endl
            << TLP_VERSION_HEADER << " " << TULIP_RELEASE  << std::endl;

  StackWalkerMinGW sw;
  sw.setContext(info->ContextRecord);
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