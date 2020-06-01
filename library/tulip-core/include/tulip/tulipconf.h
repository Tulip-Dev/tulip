/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPCONF_H
#define TULIPCONF_H

#include <cstddef>

/**
 * @brief this file contains various helper macros and functions to have a true cross-platform
 * compilation.
 *
 */

#define STRINGIFY(PARAM) STRINGIFY_INTERNAL(PARAM)
#define STRINGIFY_INTERNAL(PARAM) #PARAM

// some useful typedefs
typedef unsigned int uint;
typedef unsigned char uchar;

#if defined(_MSC_VER)
// disable some annoying Visual Studio warnings
#pragma warning(disable : 4251) // member is not dllexport
#pragma warning(disable : 4267) // conversion from 'size_t' to 'type', possible loss of data
#pragma warning(disable : 4275) // base class is not dllexport
#pragma warning(disable : 4244) // conversion to (or from) smaller integer type
#pragma warning(disable : 4355) //'this' pointer used in initializer list
#pragma warning(disable : 4800) // non-bool value coerced into bool (e.g. bool a = 5;)
#pragma warning(disable : 4503) // decorated name too long, truncated
#pragma warning(disable : 4344) // template specialisation results in different function being
                                // called (getProperty<>)

// disable deprecated warnings when compiling the tulip dlls, as MSVC is overly verbose with
// deprecation
// (even if a deprecated function is not used, warnings are issued)
#if defined(DLL_TULIP) || defined(DLL_TULIP_GL) || defined(DLL_TULIP_QT) || defined(DLL_TULIP_QT2)
#pragma warning(disable : 4996) // deprecated functions
#endif

#define _DEPRECATED __declspec(deprecated)
#define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef _DEPRECATED type deprecated_type

#define __PRETTY_FUNCTION__ __FUNCTION__ // MSVC has a different name for pretty_function
#define strcasecmp stricmp               // strcasecmp does not exists for VC, workaround

#include <cmath>

inline double fabs(int i) {
  return std::fabs(double(i));
}

// Visual Studio 2013 improved C99 support, no need to redefine some cmath functions
#if _MSC_VER < 1800

// MSVC needs explicit casting of ints to double, float or long double.
// Let's just pretend he does not.
#include <cstdlib>

inline double sqrt(int i) {
  return std::sqrt(double(i));
}

inline double sqrt(unsigned int i) {
  return std::sqrt(double(i));
}

inline double log(int i) {
  return std::log(double(i));
}

inline double log(unsigned int i) {
  return std::log(double(i));
}

inline double floor(int i) {
  return std::floor(double(i));
}

inline double floor(unsigned int i) {
  return std::floor(double(i));
}

inline double round(double d) {
  return std::floor(d + 0.5);
}

inline float strtof(const char *cptr, char **endptr) {
  return std::strtod(cptr, endptr);
}

// C99 features, and VC++ does not support C99. workaround this.
#define isnan(x) ((x) != (x))
#define rint(arg) arg > 0 ? int(std::floor(double(arg))) : int(std::ceil(double(arg)))

#if _MSC_VER <= 1600
inline double log1p(double x) {
  return log(x + 1);
}
#endif

#else // _MSC_VER < 1800

// for std::min and std::max
#include <algorithm>

#endif // _MSC_VER < 1800

// clang does not define __GNUC_MINOR__, thus having a separate clang #elif seems cleaner than
// adding defined() in the #else
#elif __clang__
#define _DEPRECATED __attribute__((deprecated))
#define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef type deprecated_type _DEPRECATED

// for GCC 4.X
#else
#define _DEPRECATED __attribute__((deprecated))
#define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef type deprecated_type _DEPRECATED
#endif

// WIN32 specific defines: dllimport and dllexport stuff
#ifdef _WIN32
#ifdef DLL_TULIP
#define TLP_SCOPE __declspec(dllexport)
#else
#define TLP_SCOPE __declspec(dllimport)
#endif
#endif
#ifndef TLP_SCOPE
#define TLP_SCOPE
#endif

#ifdef _WIN32
#ifdef DLL_TULIP_GL
#define TLP_GL_SCOPE __declspec(dllexport)
#else
#define TLP_GL_SCOPE __declspec(dllimport)
#endif
#endif
#ifndef TLP_GL_SCOPE
#define TLP_GL_SCOPE
#endif

#ifdef _WIN32
#ifdef DLL_TULIP_QT
#define TLP_QT_SCOPE __declspec(dllexport)
#else
#define TLP_QT_SCOPE __declspec(dllimport)
#endif
#endif
#ifndef TLP_QT_SCOPE
#define TLP_QT_SCOPE
#endif

#ifdef _WIN32
#ifdef DLL_TULIP_PYTHON
#define TLP_PYTHON_SCOPE __declspec(dllexport)
#else
#define TLP_PYTHON_SCOPE __declspec(dllimport)
#endif
#endif
#ifndef TLP_PYTHON_SCOPE
#define TLP_PYTHON_SCOPE
#endif

#include <ostream>

namespace tlp {
/**
 *
 * @brief return the ostream used for the output of debug messages
 */
extern TLP_SCOPE std::ostream &debug();
/**
 *
 * @brief set the ostream used for the output debug messages
 */
extern TLP_SCOPE void setDebugOutput(std::ostream &os);
/**
 *
 * @brief return the ostream used for the output of warning messages
 */
extern TLP_SCOPE std::ostream &warning();
/**
 *
 * @brief set the ostream used for the output of warning messages
 */
extern TLP_SCOPE void setWarningOutput(std::ostream &os);
/**
 *
 * @brief allow to enable/disable the output of warning messages
 */
extern TLP_SCOPE void enableWarningOutput(bool);
/**
 *
 * @brief indicate if the output of warning messages is enabled
 */
extern TLP_SCOPE bool isWarningOutputEnabled();
/**
 *
 * @brief return the ostream used for the output of error messages
 */
extern TLP_SCOPE std::ostream &error();
/**
 *
 * @brief set the ostream used for the output of error messages
 */
extern TLP_SCOPE void setErrorOutput(std::ostream &os);

/**
 *
 * @brief return the TULIP_VERSION value
 */
extern TLP_SCOPE std::string getTulipVersion();
} // namespace tlp

#endif // TULIPCONF_H
///@endcond
