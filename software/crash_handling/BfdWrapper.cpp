/*
 * BfdWrapper.cpp
 *
 *  Created on: Aug 14, 2011
 *      Author: antoine
 */


#include "BfdWrapper.h"

#include <string>
#include <iostream>
#include <cxxabi.h>
#include <sys/stat.h>

#define INRANGE(foo,bar,baz) (foo(bar))&&((bar)baz)

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
    std::cerr << "No symbol table in " << bfd_get_filename(abfd) << std::endl;
    return symbol_table;
  }

  if (useMini) {
    *nSymbols = bfd_read_minisymbols(abfd, false, reinterpret_cast<void **>(&symbol_table), symbolSize);

    if (*nSymbols == 0) {
      *isDynamic = true;
      *nSymbols = bfd_read_minisymbols(abfd, true, reinterpret_cast<void **>(&symbol_table), symbolSize);
    }

    if (*nSymbols < 0) {
      std::cerr << "Error (bfd_read_minisymbols) in " << bfd_get_filename(abfd) << std::endl;
      return symbol_table;
    }
  }
  else {

    long storage_needed = bfd_get_symtab_upper_bound(abfd);

    if (storage_needed < 0) {
      std::cerr << "Error (bfd_get_symtab_upper_bound) slurping symbol table from " << bfd_get_filename(abfd) << std::endl;
      return symbol_table;
    }

    if (storage_needed != 0)
      symbol_table = reinterpret_cast<asymbol **>(malloc(storage_needed));

    *nSymbols = bfd_canonicalize_symtab(abfd, symbol_table);

    if (*nSymbols < 0) {
      std::cerr << "Error (bfd_canonicalize_symtab) slurping symbol table from : " << bfd_get_filename(abfd) << std::endl;
      return symbol_table;
    }
  }

  if (*nSymbols == 0)
    std::cerr << "No symbols in " << bfd_get_filename(abfd) << std::endl;

  return symbol_table;
}

int file_exist (const char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

#ifdef __MINGW32__
static
DWORD GetModuleBase(DWORD dwAddress) {
  MEMORY_BASIC_INFORMATION Buffer;
  return VirtualQuery((LPCVOID) dwAddress, &Buffer, sizeof(Buffer)) ? (DWORD) Buffer.AllocationBase : 0;
}
#endif

#define PATH_MAX 1024

static bool bfdInit = false;

BfdWrapper::BfdWrapper(const char *dsoName) :
  abfd(NULL), textSection(NULL), symbolTable(NULL),
  nSymbols(0), symbolSize(0), isMini(true), isDynamic(false),
  scratchSymbol(NULL), relocationOffset(-1) {

  filePath = dsoName;

  std::string pathName = dsoName;

#ifndef __MINGW32__

  if (strchr(pathName.c_str(), '/') == NULL) {

    const char *PATH = getenv("PATH");
    char buf[PATH_MAX+2];
    const char *dir = PATH;

    while (1) {
      const char *dirend = dir;

      while (*dirend != '\0' && *dirend != ':')
        dirend++;

      if (dirend != dir) {
        snprintf(buf, sizeof(buf), "%.*s/%s", (int)(dirend-dir), dir, pathName.c_str());

        if (file_exist(buf)) {
          break;
        }
      }

      if (*dirend == '\0') {
        buf[0] = '\0';
        break;
      }

      dir = dirend+1;
    }

    if (buf[0] != '\0') {
      filePath = buf;
    }
    else {
      std::cerr << "Can't find " << pathName << " in $PATH\n" << std::endl;
      return;
    }
  }

#endif

  if (!bfdInit) {
    bfd_init();
    bfdInit = true;
  }

  abfd = bfd_openr(filePath, NULL);

  if (abfd == NULL) {
    std::cerr << "Can't open file " << dsoName << std::endl;
    return;
  }

  abfd->flags |= BFD_DECOMPRESS;

  if (!bfd_check_format(abfd, bfd_object)) {
    std::cerr << "Can't open file " << bfd_get_filename(abfd) << std::endl;
    bfd_close(abfd);
    return;
  }

  textSection = bfd_get_section_by_name(abfd, ".text");

  if (textSection == NULL) {
    std::cerr << "Can't find .text section in " << bfd_get_filename(abfd) << std::endl;
    bfd_close(abfd);
    return;
  }

  isMini = true;
  nSymbols = 0;
  symbolSize = 0;
  isDynamic = false;
  symbolTable = slurp_symtab(abfd, isMini, &nSymbols, &symbolSize, &isDynamic);

  if ((bfd_get_section_flags(abfd, textSection) & SEC_ALLOC) == 0) {
    std::cerr << "SEC_ALLOC flag not set on .text section (whatever that means) in " << bfd_get_filename(abfd) << std::endl;
    free(symbolTable);
    bfd_close(abfd);
    return;
  }

  asymbol *scratchSymbol = bfd_make_empty_symbol(abfd);

  if (scratchSymbol == NULL) {
    std::cerr << "Error (bfd_make_empty_symbol) in " << bfd_get_filename(abfd) << std::endl;
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

std::pair<const char *, unsigned int> BfdWrapper::getFileAndLineForAddress(const char *mangledSymbol, const int64_t runtimeAddr, const int64_t runtimeOffset) {
  std::pair<const char *, unsigned int> ret = std::make_pair("", 0);

  if (!isMini || symbolSize == 0)
    return ret;

  bfd_byte *from = (bfd_byte *)symbolTable;
  bfd_byte *fromend = from + nSymbols * symbolSize;
  int index = 0;

  for (; from < fromend; from += symbolSize, index++) {
    asymbol *sym = bfd_minisymbol_to_symbol(abfd, isDynamic, from, scratchSymbol);

    if (sym == NULL) {
      std::cerr << "Error (bfd_minisymbol_to_symbol) in " << bfd_get_filename(abfd) << std::endl;
      return ret;
    }

    symbol_info syminfo;
    bfd_get_symbol_info(abfd, sym, &syminfo);

    if (syminfo.type == 'T' || syminfo.type == 'W') {
      int status;

      bool matched = std::string(syminfo.name) == std::string(mangledSymbol);

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
          std::cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << std::endl;
          return ret;
        }

        if (!bfd_find_nearest_line(abfd, textSection, symbolTable, unrelocatedAddr - textSection_vma - 1, &fileName, &funcName, &lineno)) {
          std::cerr << "Can't find line for address " << std::hex << (unsigned long long)relocatedAddr << " <- " << (unsigned long long)unrelocatedAddr << std::dec << std::endl;
          return ret;
        }
        else {
          if (fileName) {
            ret = std::make_pair(fileName, lineno);
          }

          return ret;
        }
      }
    }
  }

  return ret;
}

#else

std::pair<const char *, unsigned int> BfdWrapper::getFileAndLineForAddress(const int64_t runtimeAddr) {

  const char *funcName = "";
  const char *fileName = "";
  unsigned int lineno = 0;

  int64_t symbolOffset = runtimeAddr - GetModuleBase(runtimeAddr)  - 0x1000 - 1;
  bfd_size_type textSection_size = bfd_section_size(abfd, textSection);

  if (!INRANGE((int64_t)0 <=, symbolOffset, <= (int64_t)textSection_size)) {
    std::cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << std::endl;
  }
  else {
    bfd_find_nearest_line(abfd, textSection, symbolTable, symbolOffset, &fileName, &funcName, &lineno);
  }

  std::pair<const char *, unsigned int> ret = std::make_pair(fileName, lineno);
  return ret;
}

const char *BfdWrapper::getFunctionForAddress(const int64_t runtimeAddr) {

  const char *funcName = "";
  const char *fileName = "";
  unsigned int lineno = 0;

  int64_t symbolOffset = runtimeAddr - GetModuleBase(runtimeAddr) - 0x1000 - 1;
  bfd_size_type textSection_size = bfd_section_size(abfd, textSection);

  if (!INRANGE((int64_t)0 <=, symbolOffset, <= (int64_t)textSection_size)) {
    std::cerr << "Trying to look up an address that's outside of the range of the text section of " << filePath << "... usually this means the executable or DSO in question has changed since the stack trace was generated" << std::endl;
    return funcName;
  }

  bfd_find_nearest_line(abfd, textSection, symbolTable, symbolOffset, &fileName, &funcName, &lineno);
  return funcName;
}

#endif
