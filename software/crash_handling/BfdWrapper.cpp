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

#include "BfdWrapper.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <cxxabi.h>
#include <sys/stat.h>

#define INRANGE(foo,bar,baz) (foo(bar))&&((bar)baz)

using namespace std;

static asymbol **
slurp_symtab(bfd *abfd, bool useMini,
             long *nSymbols,
             unsigned int *symbolSize,
             bool *isDynamic) {
  *nSymbols = 0;
  *symbolSize = 0;
  *isDynamic = false;
  asymbol **symbol_table = NULL;

  if ((bfd_get_file_flags (abfd) & HAS_SYMS) == 0) {
    *nSymbols = 0;
    return symbol_table;
  }

  if (useMini) {
    *nSymbols = bfd_read_minisymbols(abfd, false, reinterpret_cast<void **>(&symbol_table), symbolSize);

    if (*nSymbols == 0) {
      *isDynamic = true;
      *nSymbols = bfd_read_minisymbols(abfd, true, reinterpret_cast<void **>(&symbol_table), symbolSize);
    }

    if (*nSymbols < 0) {
      cerr << "Error (bfd_read_minisymbols) in " << bfd_get_filename(abfd) << endl;
      return symbol_table;
    }
  }
  else {

    long storage_needed = bfd_get_symtab_upper_bound(abfd);

    if (storage_needed < 0) {
      cerr << "Error (bfd_get_symtab_upper_bound) slurping symbol table from " << bfd_get_filename(abfd) << endl;
      return symbol_table;
    }

    if (storage_needed != 0)
      symbol_table = reinterpret_cast<asymbol **>(malloc(storage_needed));

    *nSymbols = bfd_canonicalize_symtab(abfd, symbol_table);

    if (*nSymbols < 0) {
      cerr << "Error (bfd_canonicalize_symtab) slurping symbol table from : " << bfd_get_filename(abfd) << endl;
      return symbol_table;
    }
  }

  if (*nSymbols == 0)
    cerr << "No symbols in " << bfd_get_filename(abfd) << endl;

  return symbol_table;
}

int file_exist (const std::string &filename) {
  struct stat   buffer;
  return (stat (filename.c_str(), &buffer) == 0);
}

#ifdef __MINGW32__
static
DWORD GetModuleBase(DWORD dwAddress) {
  MEMORY_BASIC_INFORMATION Buffer;
#ifndef I64
  return VirtualQuery(reinterpret_cast<LPCVOID>(dwAddress), &Buffer, sizeof(Buffer)) ? reinterpret_cast<DWORD>(Buffer.AllocationBase) : 0;
#else
  return VirtualQuery(reinterpret_cast<LPCVOID>(dwAddress), &Buffer, sizeof(Buffer)) ? reinterpret_cast<DWORD64>(Buffer.AllocationBase) : 0;
#endif
}
#else

static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
  string::size_type lastPos = 0;
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos) {
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    if (pos != string::npos)
      lastPos=pos+1;
    else
      lastPos=string::npos;

    pos = str.find_first_of(delimiters, lastPos);
  }
}

#endif

static bool bfdInit = false;

BfdWrapper::BfdWrapper(const char *dsoName) :
  filePath(dsoName), abfd(NULL), textSection(NULL), symbolTable(NULL),
  nSymbols(0), symbolSize(0), isMini(true), isDynamic(false),
  scratchSymbol(NULL), relocationOffset(-1) {

#ifndef __MINGW32__

  // try to find the absolute path of the shared library or executable
  if (!file_exist(filePath)) {

    // if no / character in the filename, it surely is an executable
    // whose location is in the PATH environment variable
    if (filePath.find('/') == string::npos) {
      const string PATH = getenv("PATH");
      vector<string> paths;

      tokenize(PATH, paths, ":");

      bool absolutePathFound = false;

      for (size_t i = 0 ; i < paths.size() ; ++i) {
        if (file_exist((paths[i]+"/"+filePath))) {
          filePath = paths[i]+"/"+filePath;
          absolutePathFound = true;
          break;
        }
      }

      if (!absolutePathFound) {
        cerr << "Can't find " << filePath << " in $PATH\n" << endl;
        return;
      }
    }

    string pwd(getenv("PWD"));

    // maybe it's a relative path otherwise
    if (file_exist(pwd+"/"+filePath)) {
      filePath = pwd + "/" + filePath;
    }
  }

#endif

  if (!bfdInit) {
    bfd_init();
    bfdInit = true;
  }

  abfd = bfd_openr(filePath.c_str(), NULL);

  if (abfd == NULL) {
    cerr << "Can't open file " << filePath << endl;
    return;
  }

  if (!bfd_check_format(abfd, bfd_object)) {
    cerr << "Can't open file " << bfd_get_filename(abfd) << endl;
    bfd_close(abfd);
    return;
  }

  textSection = bfd_get_section_by_name(abfd, ".text");

  if (textSection == NULL) {
    cerr << "Can't find .text section in " << bfd_get_filename(abfd) << endl;
    bfd_close(abfd);
    return;
  }

  isMini = true;
  nSymbols = 0;
  symbolSize = 0;
  isDynamic = false;
  symbolTable = slurp_symtab(abfd, isMini, &nSymbols, &symbolSize, &isDynamic);

  if ((bfd_get_section_flags(abfd, textSection) & SEC_ALLOC) == 0) {
    cerr << "SEC_ALLOC flag not set on .text section (whatever that means) in " << bfd_get_filename(abfd) << endl;
    free(symbolTable);
    bfd_close(abfd);
    return;
  }

  asymbol *scratchSymbol = bfd_make_empty_symbol(abfd);

  if (scratchSymbol == NULL) {
    cerr << "Error (bfd_make_empty_symbol) in " << bfd_get_filename(abfd) << endl;
    free(symbolTable);
    bfd_close(abfd);
    return;
  }
}

BfdWrapper::~BfdWrapper() {
  if (symbolTable != NULL) {
    free(symbolTable);
  }

  if (abfd != NULL) {
    bfd_close(abfd);
  }
}

#ifndef __MINGW32__

pair<const char *, unsigned int> BfdWrapper::getFileAndLineForAddress(const char *mangledSymbol, const int64_t runtimeAddr, const int64_t runtimeOffset) {
  pair<const char *, unsigned int> ret = make_pair("", 0);

  if (!abfd || !isMini || symbolSize == 0)
    return ret;

  bfd_byte *from = (bfd_byte *)symbolTable;
  bfd_byte *fromend = from + nSymbols * symbolSize;
  int index = 0;

  for (; from < fromend; from += symbolSize, index++) {
    asymbol *sym = bfd_minisymbol_to_symbol(abfd, isDynamic, from, scratchSymbol);

    if (sym == NULL) {
      cerr << "Error (bfd_minisymbol_to_symbol) in " << bfd_get_filename(abfd) << endl;
      return ret;
    }

    symbol_info syminfo;
    bfd_get_symbol_info(abfd, sym, &syminfo);

    if (syminfo.type == 'T' || syminfo.type == 'W') {
      bool matched = string(syminfo.name) == string(mangledSymbol);

      if (matched) {

        int64_t relocatedSymbolAddress = runtimeAddr - runtimeOffset;
        int64_t unrelocatedSymbolAddress = syminfo.value;

        relocationOffset = relocatedSymbolAddress - unrelocatedSymbolAddress;

        int64_t relocatedAddr = runtimeAddr;
        int64_t unrelocatedAddr = relocatedAddr;

        if (relocationOffset != -1)
          unrelocatedAddr -= relocationOffset;

        const char *funcName = NULL;
        const char *fileName = NULL;
        unsigned int lineno = 0;

        bfd_vma textSection_vma = bfd_get_section_vma(abfd, textSection);
        bfd_size_type textSection_size = bfd_section_size(abfd, textSection);

        if (!INRANGE((int64_t)textSection_vma <=, unrelocatedAddr, <= (int64_t)(textSection_vma+textSection_size))) {
          cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << endl;
          return ret;
        }

        if (!bfd_find_nearest_line(abfd, textSection, symbolTable, unrelocatedAddr - textSection_vma - 1, &fileName, &funcName, &lineno)) {
          cerr << "Can't find line for address " << hex << (unsigned long long)relocatedAddr << " <- " << (unsigned long long)unrelocatedAddr << dec << endl;
          return ret;
        }
        else {
          if (fileName) {
            ret = make_pair(fileName, lineno);
          }

          return ret;
        }
      }
    }
  }

  return ret;
}

#else

pair<const char *, unsigned int> BfdWrapper::getFileAndLineForAddress(const int64_t runtimeAddr) {

  pair<const char *, unsigned int> ret = make_pair("", 0);

  if (!abfd)
    return ret;

  const char *funcName = "";
  const char *fileName = "";
  unsigned int lineno = 0;

  int64_t symbolOffset = runtimeAddr - GetModuleBase(runtimeAddr)  - 0x1000 - 1;
  bfd_size_type textSection_size = bfd_section_size(abfd, textSection);

  if (!INRANGE((int64_t)0 <=, symbolOffset, <= (int64_t)textSection_size)) {
    cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << endl;
  }
  else {
    bfd_find_nearest_line(abfd, textSection, symbolTable, symbolOffset, &fileName, &funcName, &lineno);
  }

  ret = make_pair(fileName, lineno);
  return ret;
}

const char *BfdWrapper::getFunctionForAddress(const int64_t runtimeAddr) {

  const char *funcName = "";
  const char *fileName = "";
  unsigned int lineno = 0;

  if (!abfd)
    return funcName;

  int64_t symbolOffset = runtimeAddr - GetModuleBase(runtimeAddr) - 0x1000 - 1;
  bfd_size_type textSection_size = bfd_section_size(abfd, textSection);

  if (!INRANGE((int64_t)0 <=, symbolOffset, <= (int64_t)textSection_size)) {
    cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << endl;
    return funcName;
  }

  bfd_find_nearest_line(abfd, textSection, symbolTable, symbolOffset, &fileName, &funcName, &lineno);
  return funcName;
}

#endif
