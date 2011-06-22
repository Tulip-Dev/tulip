#ifndef CRASHHANDLING_H
#define CRASHHANDLING_H

#include "SystemDefinition.h"
#include <tulip/TulipRelease.h>

#if defined(USE_GOOGLE_BREAKPAD)

#define BREAKPAD_PLATEFORM_HEADER "TLP_PLATEFORM"
#define BREAKPAD_ARCH_HEADER "TLP_ARCH"
#define BREAKPAD_COMPILER_HEADER "TLP_COMPILER"
#define BREAKPAD_VERSION_HEADER "TLP_VERSION"
#define BREAKPAD_DUMP_HEADER "TLP_DUMP"

#if defined(__linux)
#include <client/linux/handler/exception_handler.h>

static bool dumpCallback(const char* dump_path, const char* minidump_id, void* context, bool succeeded) {
  printf("%s %s\n%s %s\n%s %s\n%s %s\n%s %s/%s.dmp\n",
         BREAKPAD_PLATEFORM_HEADER,OS_PLATFORM,
         BREAKPAD_ARCH_HEADER,OS_ARCHITECTURE,
         BREAKPAD_COMPILER_HEADER,OS_COMPILER,
         BREAKPAD_VERSION_HEADER,TULIP_RELEASE,
         BREAKPAD_DUMP_HEADER,dump_path,minidump_id);
  return succeeded;
}

static void start_crash_handler() {
  new google_breakpad::ExceptionHandler("/tmp", NULL, dumpCallback, NULL, true);
}
#elif defined(_WIN32)

#else /* __linux */
#error USE_GOOGLE_BREAKPAD is defined whereas neither __linux or _WIN32 are.
#endif /* __linux */

#else /* USE_GOOGLE_BREAKPAD */
static void start_crash_handler() {}
#endif /* USE_GOOGLE_BREAKPAD */

#endif // CRASHHANDLING_H
