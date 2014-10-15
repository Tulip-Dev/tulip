/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include "StackWalker.h"


#include <sstream>
#include <cstring>

#if defined(__unix__) || defined(__APPLE__)

#include <cxxabi.h>
#ifndef __APPLE__
#include <execinfo.h>
#else
#include <dlfcn.h>
#include <sys/stat.h>

int backtrace(void **buffer, int size) {
  void **frame = reinterpret_cast<void **>(__builtin_frame_address(0));
  void **bp = reinterpret_cast<void **>(*frame);
  void *ip = frame[1];
  int i;

  for (i = 0; bp && ip && i < size ; ++i) {
    ip = bp[1];
    buffer[i] = ip;
    bp = reinterpret_cast<void**>(bp[0]);
  }

  return i;
}

char *getStackFrameDetails(void *address) {
  Dl_info dli;
  char tmp[1024];

  if (dladdr(address, &dli)) {
    int64_t function_offset = reinterpret_cast<int64_t>(address) - reinterpret_cast<int64_t>(dli.dli_saddr);
    sprintf(tmp, "%s(%s+%p)[%p]", dli.dli_fname, dli.dli_sname, reinterpret_cast<void *>(function_offset), address);
  }
  else {
    sprintf(tmp, "%s(%s+%s)[%p]", "???", "???", "???", address);
  }

  char *ret = new char[strlen(tmp)+1];
  strcpy(ret, tmp);
  return ret;
}

char **backtrace_symbols(void *const *buffer, int size) {
  char **ret = new char*[size];

  for (int i = 0 ; i < size ; ++i) {
    ret[i] = getStackFrameDetails(buffer[i]);
  }

  return ret;
}

int64_t getOffsetInExecutable(void *address) {
  Dl_info dli;
  int64_t ret = 0;

  if (dladdr(address, &dli)) {
    int64_t base = reinterpret_cast<int64_t>(dli.dli_fbase);
    ret = reinterpret_cast<int64_t>(address);

    if (base != 0x1000)
      ret -= base;
  }

  return ret;
}

std::string pOpen(const std::string &cmd) {
  const int MAX =1024;
  FILE *cmdPipe = popen(cmd.c_str(), "r+");

  if (cmdPipe) {
    char buffer[MAX];
    char *s = fgets(buffer, MAX - 1, cmdPipe);
    pclose(cmdPipe);

    if (s == NULL) {
      return "";
    }
    else {
      std::string str(buffer);
      return str.substr(0, str.size() - 1);
    }
  }

  return "";
}

std::pair<const char *, unsigned int> extractFileAndLine(const std::string &atosOutput) {
  std::string ext[5] = {".cpp", ".cc", ".c", ".hpp", ".h"};
  std::pair<const char *, unsigned int> ret = std::make_pair("", 0);

  for (size_t i = 0 ; i < 5 ; ++i) {
    size_t pos = atosOutput.find(ext[i]);

    if (pos != std::string::npos) {
      size_t pos2 = atosOutput.find(':', pos);
      size_t pos3 = atosOutput.find(')', pos2);

      while (atosOutput[pos] != '(') {
        --pos;
      }

      ret = std::make_pair(atosOutput.substr(pos+1, pos2-pos-1).c_str(), atoi(atosOutput.substr(pos2+1, pos3-pos2-1).c_str()));
      break;
    }
  }

  return ret;
}

int file_exist(const std::string &filename) {
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

#endif

StackWalkerGCC::StackWalkerGCC() {}

StackWalkerGCC::~StackWalkerGCC() {
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *>::iterator it = bfdMap.begin();

  for ( ; it != bfdMap.end() ; ++it) {
    delete it->second;
  }

#endif
}

#define MAX_BACKTRACE_SIZE 128

void StackWalkerGCC::printCallStack(std::ostream &os, unsigned int maxDepth) {

  void *array[MAX_BACKTRACE_SIZE];
  int size = backtrace(array, MAX_BACKTRACE_SIZE);
  char ** messages = backtrace_symbols(array, size);

  if (messages == NULL || size < 2)
    return;

  std::ostringstream oss;
  oss << callerAddress;
  std::string callerAddressStr = oss.str();

  int i = 1;

#ifndef __APPLE__

  while (i < size) {
    std::string msg(messages[i]);

    if (msg.find(callerAddressStr) != std::string::npos) {
      break;
    }

    ++i;
  }

#else

  while (i < size) {
    std::string msg(messages[i]);

    if (msg.find("_sigtramp") != std::string::npos) {
      ++i;
      break;
    }

    ++i;
  }

  if (i == size) i = 1;

  std::string msg(messages[i]);

  if (msg.find("???") != std::string::npos) {
    array[i] = callerAddress;
    messages[i] = getStackFrameDetails(callerAddress);
  }

#endif

  int offset = i;

  for (; i < size ; ++i) {
    char *mangled_name = 0, *runtime_offset = 0,
          *offset_end = 0, *runtime_addr = 0,
           *runtime_addr_end = 0, *dsoName = 0;

    if (static_cast<unsigned int>(i) > maxDepth)
      return;

    for (char *p = messages[i]; *p; ++p) {
      if (*p == '(') {
        mangled_name = p;
      }
      else if (*p == '+') {
        runtime_offset = p;
      }
      else if (*p == ')') {
        offset_end = p;
      }
      else if (*p == '[') {
        runtime_addr = p;
      }
      else if (*p == ']') {
        runtime_addr_end = p;
      }
    }

    dsoName = messages[i];

    if (mangled_name)
      *mangled_name++ = '\0';

    if (runtime_offset)
      *runtime_offset++ = '\0';

    *offset_end++ = '\0';
    *runtime_addr++ = '\0';
    *runtime_addr_end = '\0';

    if (mangled_name && runtime_offset &&
        mangled_name < runtime_offset) {

      std::string dsoNameStr(dsoName);

      int status;
      char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

      char *end;
      int64_t runtimeAddr = static_cast<int64_t>(strtoll(runtime_addr, &end, 16));
      int64_t runtimeOffset = static_cast<int64_t>(strtoll(runtime_offset, &end, 0));

      if (runtimeAddr == 1 && i == (size - 1))
        break;

      std::pair<const char *, unsigned int> infos = std::make_pair("", 0);

#ifdef HAVE_BFD

      if (bfdMap.find(dsoNameStr) == bfdMap.end()) {
        bfdMap[dsoNameStr] = new BfdWrapper(dsoName);
      }

      dsoName = const_cast<char*>(bfdMap[dsoNameStr]->getDsoAbsolutePath().c_str());
      infos =bfdMap[dsoNameStr]->getFileAndLineForAddress(mangled_name, runtimeAddr, runtimeOffset);
#endif

#ifdef __APPLE__

      if (file_exist("/usr/bin/atos") && file_exist(dsoName)) {
        int64_t exOffset = getOffsetInExecutable(array[i]);
        std::ostringstream oss;
        oss << "atos -o " << dsoName;
#ifdef I64
        oss << " -arch x86_64 ";
#else
        oss << " -arch i386 ";
#endif
        oss << "0x" << std::hex << exOffset;
        std::string atos = pOpen(oss.str());
        infos = extractFileAndLine(atos);
      }

#endif

      if (status == 0) {
        if (std::string(infos.first) == "") {
          printFrameInfos(os, i - offset, runtimeAddr, dsoName, real_name, runtimeOffset);
        }
        else {
          printFrameInfos(os, i - offset, runtimeAddr, dsoName, real_name, runtimeOffset, infos.first, infos.second);
        }
      }
      else {
        if (std::string(infos.first) == "") {
          printFrameInfos(os, i - offset, runtimeAddr, dsoName, mangled_name, runtimeOffset);
        }
        else {
          printFrameInfos(os, i - offset, runtimeAddr, dsoName, mangled_name, runtimeOffset, infos.first, infos.second);
        }
      }

      free(real_name);
    }
  }

  free(messages);
}

#elif defined(__MINGW32__)

#include <imagehlp.h>
#include <cxxabi.h>

StackWalkerMinGW::StackWalkerMinGW() {}

StackWalkerMinGW::~StackWalkerMinGW() {
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *>::iterator it = bfdMap.begin();

  for ( ; it != bfdMap.end() ; ++it) {
    delete it->second;
  }

#endif
}

void StackWalkerMinGW::printCallStack(std::ostream &os, unsigned int maxDepth) {
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  SymSetOptions(SymGetOptions() | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);

  if (!SymInitialize(process, 0, TRUE)) {
    std::cerr << "Failed to init symbol context" << std::endl;
    return;
  }

  char procname[MAX_PATH];
  GetModuleFileNameA(NULL, procname, sizeof procname);

  STACKFRAME frame;
  memset(&frame,0,sizeof(frame));

#ifndef I64
  DWORD machine = IMAGE_FILE_MACHINE_I386;
  frame.AddrPC.Offset    = context->Eip;
  frame.AddrStack.Offset = context->Esp;
  frame.AddrFrame.Offset = context->Ebp;
#else
  DWORD machine = IMAGE_FILE_MACHINE_AMD64;
  frame.AddrPC.Offset    = context->Rip;
  frame.AddrStack.Offset = context->Rsp;
  frame.AddrFrame.Offset = context->Rbp;
#endif

  frame.AddrPC.Mode      = AddrModeFlat;
  frame.AddrStack.Mode   = AddrModeFlat;
  frame.AddrFrame.Mode   = AddrModeFlat;

  char symbol_buffer[sizeof(IMAGEHLP_SYMBOL) + 255];
  char module_name_raw[MAX_PATH];

  int depth = maxDepth;

  while(StackWalk(machine,
                  process,
                  thread,
                  &frame,
                  context,
                  0,
                  SymFunctionTableAccess,
                  SymGetModuleBase, 0)) {

    --depth;

    if (depth < 0)
      break;

    IMAGEHLP_SYMBOL *symbol = reinterpret_cast<IMAGEHLP_SYMBOL *>(symbol_buffer);
    symbol->SizeOfStruct = (sizeof *symbol) + 255;
    symbol->MaxNameLength = 254;

#ifndef I64
    DWORD module_base = SymGetModuleBase(process, frame.AddrPC.Offset);
#else
    DWORD64 module_base = SymGetModuleBase(process, frame.AddrPC.Offset);
#endif

    int64_t symbolOffset = frame.AddrPC.Offset - module_base  - 0x1000 - 1;

    const char * module_name = "[unknown module]";

    if (module_base &&
        GetModuleFileNameA(reinterpret_cast<HMODULE>(module_base), module_name_raw, MAX_PATH)) {
      module_name = module_name_raw;
    }

#ifdef HAVE_BFD
    std::string moduleNameStr(module_name);

    if (bfdMap.find(moduleNameStr) == bfdMap.end()) {
      bfdMap[moduleNameStr] = new BfdWrapper(module_name);
    }

#endif

    const char * func = NULL;

#ifndef I64
    DWORD dummy = 0;
#else
    DWORD64 dummy = 0;
#endif

    if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &dummy, symbol)) {
      func = symbol->Name;
    }

    if (func) {
      int status;
      const char *toDemangle = (std::string("_") + std::string(func)).c_str();
      char *realName = abi::__cxa_demangle(toDemangle, 0, 0, &status);

      if (status == 0)
        func = realName;

    }

#ifdef HAVE_BFD
    else {
      func = bfdMap[moduleNameStr]->getFunctionForAddress(frame.AddrPC.Offset);
    }

#endif

    if (func == NULL) {
      func = "";
    }

#ifdef HAVE_BFD
    std::pair<const char *, unsigned int> infos =
      bfdMap[moduleNameStr]->getFileAndLineForAddress(frame.AddrPC.Offset);
#else
    std::pair<const char *, unsigned int> infos = std::make_pair("", 0);
#endif

    if (infos.first == NULL || infos.second == 0) {
      printFrameInfos(os, maxDepth - depth - 1, frame.AddrPC.Offset, module_name, func, symbolOffset);
    }
    else {
      printFrameInfos(os, maxDepth - depth - 1, frame.AddrPC.Offset, module_name, func, symbolOffset, infos.first, infos.second);
    }
  }

  SymCleanup(process);
}

#elif defined(_MSC_VER)

#include <dbghelp.h>

StackWalkerMSVC::StackWalkerMSVC(): context(NULL) {}

StackWalkerMSVC::~StackWalkerMSVC() {}

void StackWalkerMSVC::printCallStack(std::ostream &os, unsigned int maxDepth) {

  BOOL result;
  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
  SymInitialize(process, NULL, TRUE);


  STACKFRAME        stack;
  ULONG               frame;
  IMAGEHLP_SYMBOL   *symbol = reinterpret_cast<IMAGEHLP_SYMBOL*>(malloc(sizeof(IMAGEHLP_SYMBOL)+2048*sizeof(TCHAR)));
#ifdef I64
  DWORD64             displacement = 0;
#else
  DWORD             displacement = 0;
#endif

  DWORD         displacement2 = 0;

  memset( &stack, 0, sizeof( STACKFRAME ) );

  symbol->SizeOfStruct  = sizeof( IMAGEHLP_SYMBOL );
  symbol->MaxNameLength = 2048;

#ifndef I64
  DWORD machine = IMAGE_FILE_MACHINE_I386;
  stack.AddrPC.Offset    = context->Eip;
  stack.AddrStack.Offset = context->Esp;
  stack.AddrFrame.Offset = context->Ebp;
#else
  DWORD machine = IMAGE_FILE_MACHINE_AMD64;
  stack.AddrPC.Offset    = context->Rip;
  stack.AddrStack.Offset = context->Rsp;
  stack.AddrFrame.Offset = context->Rbp;
#endif

  stack.AddrPC.Mode      = AddrModeFlat;
  stack.AddrStack.Mode   = AddrModeFlat;
  stack.AddrFrame.Mode   = AddrModeFlat;

  for( frame = 0; ; frame++ ) {
    result = StackWalk(
               machine,
               process,
               thread,
               &stack,
               context,
               NULL,
               SymFunctionTableAccess,
               SymGetModuleBase,
               NULL
             );

    if(!result) {
      break;
    }

    IMAGEHLP_MODULE image_module;
    IMAGEHLP_LINE image_line;

    image_module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
    image_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

    std::string module_name = "";
    std::string symbol_name = "";
    std::string file_name = "";
    int line = 0;

#ifndef I64
    DWORD module_base = SymGetModuleBase(process, stack.AddrPC.Offset);
#else
    DWORD64 module_base = SymGetModuleBase(process, stack.AddrPC.Offset);
#endif

    if (SymGetModuleInfo(process,stack.AddrPC.Offset, &image_module)) {
      module_name = image_module.ImageName;
    }

    if (SymGetSymFromAddr( process, stack.AddrPC.Offset, &displacement, symbol )) {
      symbol_name = symbol->Name;
    }

    if (SymGetLineFromAddr( process, stack.AddrPC.Offset - 1, &displacement2, &image_line )) {
      file_name = image_line.FileName;
      line = image_line.LineNumber;
    }

    printFrameInfos(os, frame, stack.AddrPC.Offset, module_name, symbol_name, stack.AddrPC.Offset - module_base, file_name, line);

  }
}

#endif
