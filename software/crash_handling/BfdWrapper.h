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

  const std::string &getDsoAbsolutePath() const {
    return filePath;
  }

private :

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
