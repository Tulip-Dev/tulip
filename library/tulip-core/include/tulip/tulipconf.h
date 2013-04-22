/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPCONF_H
#define TULIPCONF_H

/**
 * @brief this file contains various helper macros and functions to have a true cross-platform compilation.
 *
 */

#define STRINGIFY(PARAM) STRINGIFY_INTERNAL(PARAM)
#define STRINGIFY_INTERNAL(PARAM) #PARAM

//MSVC and GCC in c++11 mode use decltype instead of typeof
#if !defined(_MSC_VER)
#  if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define TYPEOF decltype
#  else
#    define TYPEOF typeof
#  endif
#endif

#if defined(_MSC_VER)
// disable some annoying Visual Studio warnings
#pragma warning(disable: 4251) //member is not dllexport
#pragma warning(disable: 4267) //conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable: 4275) //base class is not dllexport
#pragma warning(disable: 4244) //conversion to (or from) smaller integer type
#pragma warning(disable: 4355) //'this' pointer used in initializer list
#pragma warning(disable: 4800) //non-bool value coerced into bool (e.g. bool a = 5;)
#pragma warning(disable: 4503) //decorated name too long, truncated
#pragma warning(disable: 4344) //template specialisation results in different function being called (getProperty<>)

// disable deprecated warnings when compiling the tulip dlls, as MSVC is overly verbose with deprecation
// (even if a deprecated function is not used, warnings are issued)
#if defined(DLL_TULIP) || defined(DLL_TULIP_GL) || defined(DLL_TULIP_QT) || defined(DLL_TULIP_QT2)
#pragma warning(disable: 4996) //deprecated functions
#endif
//MSVC 2010 has a different keyword for typeof, and deprecated
#  if _MSC_VER >= 1600
#    define _DEPRECATED __declspec(deprecated)
#    define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef _DEPRECATED type deprecated_type
#    define TYPEOF decltype
#  else
//MSVC 2008 must use Boost's typeof
#    include "boost/typeof/typeof.hpp"
#    define TYPEOF BOOST_TYPEOF
#  endif

//MSVC needs explicit casting of ints ot double, float or long double. Let's just pretend he does not.
#include <cmath>
#include <cstdlib>
static double sqrt(int i) {
  return sqrt((double)i);
}
static double sqrt(unsigned int i) {
  return sqrt((double)i);
}

static double log(int i) {
  return log((double)i);
}
static double log(unsigned int i) {
  return log((double)i);
}

static double floor(int i) {
  return floor((double)i);
}
static double floor(unsigned int i) {
  return floor((double)i);
}

static double round(double d) {
  return floor(d + 0.5);
}

static double fabs(int i) {
  return fabs((double)i);
}

static float strtof(const char* cptr, char** endptr) {
  return strtod(cptr, endptr);
}

#  define __PRETTY_FUNCTION__ __FUNCTION__ //MSVC has a different name for pretty_function
#  define strcasecmp stricmp  //strcasecmp does not exists for VC, workaround
#  define isnan(x) ((x) != (x)) //you guessed it, this is a C99 feature, and VC++ does not support C99. workaroud this.
#  define rint(arg) arg > 0 ? (int)std::floor((double)arg) : (int)std::ceil((double)arg) //Hey, nother C99 feature !

//clang does not define __GNUC_MINOR__, thus having a separate clang #elif seems cleaner than adding defined() in the #else
#elif __clang__
#  define _DEPRECATED __attribute__ ((deprecated))
#  define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef type deprecated_type _DEPRECATED
#  define stdext __gnu_cxx

//for GCC 4.X
#else
#    define _DEPRECATED __attribute__ ((deprecated))
#    define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef type deprecated_type _DEPRECATED
#    define stdext __gnu_cxx
#  if  (__GNUC_MINOR__ < 4 && __GNUC__ < 4)
#    include <ext/stl_hash_fun.h>
#  elif (__GNUC_MINOR__ < 3)
#    include <ext/hash_fun.h>
#  endif
#endif

//WIN32 specific defines: dllimport and dllexport stuff
#ifdef _WIN32
#  ifdef DLL_TULIP
#    define TLP_SCOPE       __declspec(dllexport)
#  else
#    define TLP_SCOPE       __declspec(dllimport)
#  endif
#endif
#ifndef TLP_SCOPE
#  define TLP_SCOPE
#endif

#ifdef _WIN32
#  ifdef DLL_TULIP_GL
#    define TLP_GL_SCOPE       __declspec(dllexport)
#  else
#    define TLP_GL_SCOPE       __declspec(dllimport)
#  endif
#endif
#ifndef TLP_GL_SCOPE
#  define TLP_GL_SCOPE
#endif

#ifdef _WIN32
#  ifdef DLL_TULIP_QT
#    define TLP_QT_SCOPE       __declspec(dllexport)
#  else
#    define TLP_QT_SCOPE       __declspec(dllimport)
#  endif
#endif
#ifndef TLP_QT_SCOPE
#  define TLP_QT_SCOPE
#endif

#ifdef _WIN32
#  ifdef DLL_TULIP_PYTHON
#    define TLP_PYTHON_SCOPE       __declspec(dllexport)
#  else
#    define TLP_PYTHON_SCOPE       __declspec(dllimport)
#  endif
#endif
#ifndef TLP_PYTHON_SCOPE
#  define TLP_PYTHON_SCOPE
#endif

#ifdef _WIN32
#  ifdef DLL_TULIP_OGDF
#    define TLP_OGDF_SCOPE         __declspec(dllexport)
#  else
#    define TLP_OGDF_SCOPE         __declspec(dllimport)
#  endif
#endif
#ifndef TLP_OGDF_SCOPE
#  define TLP_OGDF_SCOPE
#endif

#include <ostream>

namespace tlp {
/**
 *
 * @brief return the ostream used for the output of debug messages
 */
extern TLP_SCOPE std::ostream& debug();
/**
 *
 * @brief set the ostream used for the output debug messages
 */
extern TLP_SCOPE void setDebugOutput(std::ostream& os);
/**
 *
 * @brief return the ostream used for the output of warning messages
 */
extern TLP_SCOPE std::ostream& warning();
/**
 *
 * @brief set the ostream used for the output of warning messages
 */
extern TLP_SCOPE void setWarningOutput(std::ostream& os);
/**
 *
 * @brief return the ostream used for the output of error messages
 */
extern TLP_SCOPE std::ostream& error();
/**
 *
 * @brief set the ostream used for the output of error messages
 */
extern TLP_SCOPE void setErrorOutput(std::ostream& os);
}

#endif //TULIPCONF_H
///@endcond
