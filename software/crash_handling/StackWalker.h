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

#ifndef STACKWALKER_H_
#define STACKWALKER_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <stdint.h>

#include <tulip/ConsoleUtils.h>

#ifndef _MSC_VER
#ifdef HAVE_BFD
#include "BfdWrapper.h"
#endif
#else
typedef __int64 int64_t;
#endif

class StackWalker {
public:
  virtual ~StackWalker() {}
  virtual void printCallStack(std::ostream &os, unsigned int maxDepth = 50) = 0;

  void printCallStackToStdErr(unsigned int maxDepth = 50) {
    printCallStack(std::cerr, maxDepth);
  }

  virtual void printFrameInfos(std::ostream &os, unsigned int frameId, int64_t pcAddress, const std::string& moduleName, const std::string& funcName="", int64_t symbolOffset=0, const std::string& fileName="", unsigned int line=0) {

    if (frameId%2 == 0)
      os << setTextBackgroundColor(DARK_GRAY);
    else
      os << setTextBackgroundColor(LIGHT_GRAY);

    os << bold;

    os << lightRed << std::dec << std::setfill('0') << "#" << std::setw(2) << frameId
       << lightMagenta << " 0x" << std::hex << std::setw(16) << pcAddress << lightRed << " in ";

    os << white;

    if (funcName != "") {
      os << funcName;
    }
    else {
      os << "??";
    }

    if (symbolOffset != 0) {
      os << lightMagenta << " (+0x" << std::hex << symbolOffset << ")";
    }

    if (fileName != "") {
      os << lightRed << " at " << lightGreen << fileName << ":" << std::dec << lightYellow << line;
    }

    if (moduleName != "") {
      os << lightRed << " from " << lightCyan << moduleName;
    }
    else {
      os << lightRed << " from " << lightGreen << "??";
    }

    os << fillToEndOfLine << defaultTextColor << std::endl;

  }

};

#if defined(__unix__) || defined(__APPLE__)

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
