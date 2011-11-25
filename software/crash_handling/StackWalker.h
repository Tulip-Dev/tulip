/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef STACKWALKER_H_
#define STACKWALKER_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>
#include <cstdlib>

#ifndef _MSC_VER
#ifdef HAVE_BFD
#include "BfdWrapper.h"
#endif
#endif

#ifdef WIN32
#include <windows.h>
#endif

enum FontType {NORMAL=0, BOLD=1};

enum TextColor {BLACK=0, RED=1, GREEN=2, YELLOW=3, BLUE=4,
                MAGENTA=5, CYAN=6, WHITE=7
               };

inline std::ostream& defaultColor(std::ostream &s) {
  if (s == std::cout || s == std::cerr) {
    s << "\E[0m";
  }

  return s;
}

inline std::ostream& setTextColor(std::ostream &s, FontType ft, TextColor fg, TextColor bg) {
  if (s == std::cout || s == std::cerr) {
    s << "\E[" << ft << "m\E[3" << fg << "m\E[4" << bg << "m";
  }

  return s;
}

inline std::ostream& red(std::ostream &s) {
  return setTextColor(s, BOLD, RED, BLACK);
}

inline std::ostream& green(std::ostream &s) {
  return setTextColor(s, BOLD, GREEN, BLACK);
}

inline std::ostream& blue(std::ostream &s) {
  return setTextColor(s, BOLD, BLUE, BLACK);
}

inline std::ostream& yellow(std::ostream &s) {
  return setTextColor(s, BOLD, YELLOW, BLACK);
}

inline std::ostream& white(std::ostream &s) {
  return setTextColor(s, BOLD, WHITE, BLACK);
}

inline std::ostream& magenta(std::ostream &s) {
  return setTextColor(s, BOLD, MAGENTA, BLACK);
}

inline std::ostream& cyan(std::ostream &s) {
  return setTextColor(s, BOLD, CYAN, BLACK);
}


class StackWalker {
public:
  virtual ~StackWalker() {}
  virtual void printCallStack(std::ostream &os, unsigned int maxDepth = 50) = 0;

  void printCallStackToStdErr(unsigned int maxDepth = 50) {
    printCallStack(std::cerr, maxDepth);
  }

  virtual void printFrameInfos(std::ostream &os, unsigned int frameId, int64_t pcAddress, std::string moduleName, std::string funcName="", int64_t symbolOffset=0, std::string fileName="", unsigned int line=0) {
    os << red << std::dec << std::setfill('0') << "#" << std::setw(2) << frameId
       << white << " 0x" << std::hex << std::setw(16) << pcAddress << red << " in ";

    os << blue;

    if (funcName != "") {
      os << funcName;
    }
    else {
      os << "??";
    }

    if (symbolOffset != 0) {
      os << white << " (+0x" << std::hex << symbolOffset << ")";
    }

    if (fileName != "") {
      os << red << " at " << green << fileName << ":" << std::dec << yellow << line;
    }

    if (moduleName != "") {
      os << red << " from " << cyan << moduleName << std::endl;
    }
    else {
      os << red << " from " << green << "??" << std::endl;
    }

    os << defaultColor;

  }

};


#if defined(__linux) || defined(__APPLE__)

#include <map>

class StackWalkerGCC : public StackWalker {

public:

  StackWalkerGCC();

  ~StackWalkerGCC();

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50);

  void setCallerAddress(void *callerAddress) {
    this->callerAddress = callerAddress;
  }

private:

  void *callerAddress;
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *> bfdMap;
#endif

};


#elif defined(__MINGW32__)

#include <map>

class StackWalkerMinGW : public StackWalker {
public:
  StackWalkerMinGW();
  ~StackWalkerMinGW();

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50);

  void setContext(LPCONTEXT context) {
    this->context = context;
  }

private:
  LPCONTEXT context;
#ifdef HAVE_BFD
  std::map<std::string, BfdWrapper *> bfdMap;
#endif
};

#elif defined(_MSC_VER)

class StackWalkerMSVC : public StackWalker {
public:
  StackWalkerMSVC();
  ~StackWalkerMSVC();

  void printCallStack(std::ostream &os, unsigned int maxDepth = 50);
  void setContext(CONTEXT *context) {
    this->context = context;
  }

private :
  CONTEXT *context;
};

#endif

#endif /* STACKWALKER_H_ */
