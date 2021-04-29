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
#include <string>

// On FreeBSD, the ansidecl.h header file from the binutils port, containing the ATTRIBUTE_UNUSED
// and ENUM_BITFIELD
// preprocessor macro is not installed and its use from bfd.h is also removed (see
// https://svnweb.freebsd.org/ports?view=revision&revision=366535,
// https://svnweb.freebsd.org/ports?view=revision&revision=343349,
// https://svnweb.freebsd.org/ports?view=revision&revision=336642)
// Those macro are needed for compiling a file including bfd.h, so define them as found in
// ansidecl.h
#ifdef __FreeBSD__
#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif
#ifndef ENUM_BITFIELD
#define ENUM_BITFIELD(TYPE) enum TYPE
#endif

// To link an application against libbfd from devel/binutils, one must install
// devel/gnulibiberty to resolve the missing symbols, but that port uses
// libiberty from binutils v2.19.1 which doesn't contain all the symbols from
// v2.24, in particular filename_ncmp required by libbfd.
// So add the implementation from filename_ncmp as found in the libiberty source code.
extern "C" int filename_ncmp(const char *s1, const char *s2, size_t n);

#endif

#include <bfd.h>

class BfdWrapper {

public:
  BfdWrapper(const char *dsoName);
  ~BfdWrapper();

#ifndef __MINGW32__
  std::pair<const char *, unsigned int> getFileAndLineForAddress(const char *unmangledFuncName,
                                                                 const int64_t runtimeAddr,
                                                                 const int64_t runtimeOffset);
#else
  std::pair<const char *, unsigned int> getFileAndLineForAddress(const int64_t runtimeAddr);
  const char *getFunctionForAddress(const int64_t runtimeAddr);
#endif

  const std::string &getDsoAbsolutePath() const {
    return filePath;
  }

private:
  std::string filePath;
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
