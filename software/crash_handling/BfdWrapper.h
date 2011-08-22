/*
 * BfdWrapper.h
 *
 *  Created on: Aug 14, 2011
 *      Author: antoine
 */

#ifndef BFDWRAPPER_H_
#define BFDWRAPPER_H_

#ifdef __MINGW32__
#include <windows.h>
#include <stdint.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <utility>

#include <bfd.h>

class BfdWrapper {

public :

  BfdWrapper(const char *dsoName);
  ~BfdWrapper();

#ifndef __MINGW32__
  std::pair<const char *, unsigned int> getFileAndLineForAddress(const char *unmangledFuncName, const int64_t runtimeAddr, const int64_t runtimeOffset);
#else
  std::pair<const char *, unsigned int> getFileAndLineForAddress(const int64_t runtimeAddr);
  const char *getFunctionForAddress(const int64_t runtimeAddr);
#endif

private :

  const char *filePath;
  bfd *abfd;
  asection *textSection;
  asymbol **symbolTable;
  long nSymbols;
  unsigned int symbolSize;
  bool isMini;
  bool isDynamic;
  asymbol *scratchSymbol;
  int64_t relocationOffset; // deduced later
};

#endif /* BFDWRAPPER_H_ */
