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
#ifndef TULIPCONF_H
#define TULIPCONF_H

/**
 * @brief this file contains various helper macros and functions to have a true cross-platform compilation.
 *
 */

#define STRINGIFY(PARAM) STRINGIFY_INTERNAL(PARAM)
#define STRINGIFY_INTERNAL(PARAM) #PARAM

//MSVC is the only compiler we use that does not have typeof
#if !defined(_MSC_VER)
#  if defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define TYPEOF decltype
#  else
#    define TYPEOF typeof
#  endif
#endif

#if defined(_MSC_VER)
// disable some annoying Visual Studio warnings
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma warning(disable: 4503)
// disable deprecated warnings when compiling the tulip dlls
#if defined(DLL_TULIP) || defined(DLL_TULIP_GL) || defined(DLL_TULIP_QT) || defined(DLL_TULIP_QT2)
#pragma warning(disable: 4996)
#endif
//MSVC 2010 has a different keyword for typeof, and deprecated
#  if _MSC_VER >= 1600
#    define _DEPRECATED __declspec(deprecated)
#    define TYPEOF decltype
#  else
//MSVC 2008 must use Boost's typeof
#    include "boost/typeof/typeof.hpp"
#    define TYPEOF BOOST_TYPEOF
#  endif

#  define __PRETTY_FUNCTION__ __FUNCTION__ //MSVC has a different name for pretty_function
//Need this line to keep code compatibility.
//Remove it as soon as possible.
#  include <tulip/vs_compat.h>

//for GCC 3.X
#elif (__GNUC__ < 3)
#  define stdext std
#  define _DEPRECATED
#  include <stl_hash_fun.h>

//clang does not define __GNUC_MINOR__, thus having a separate clang #elif seems cleaner than adding defined() in the #else
#elif defined __clang__
#  define _DEPRECATED __attribute__ ((deprecated))
#  define stdext __gnu_cxx

//for GCC 4.X
#else
#    define _DEPRECATED __attribute__ ((deprecated))
#    define stdext __gnu_cxx
#  if  (__GNUC_MINOR__ < 4 && __GNUC__ < 4)
#    include <ext/stl_hash_fun.h>
#  elif (__GNUC_MINOR__ < 3)
#    include <ext/hash_fun.h>
#  endif
#endif

//WIN32 specific defines: dllimport and dllexport stuff
#ifdef _WIN32
#   ifdef DLL_TULIP
#   define TLP_SCOPE         __declspec(dllexport)
#   else
#   define TLP_SCOPE       __declspec(dllimport)
#   endif
#endif
#ifndef TLP_SCOPE
#define TLP_SCOPE
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_GL
#   define TLP_GL_SCOPE         __declspec(dllexport)
#   else
#   define TLP_GL_SCOPE       __declspec(dllimport)
#   endif
#endif
#ifndef TLP_GL_SCOPE
#define TLP_GL_SCOPE
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_QT2
#   define TLP_QT_SCOPE       __declspec(dllimport)
# else
#     ifdef DLL_TULIP_QT
#     define TLP_QT_SCOPE         __declspec(dllexport)
#     else
#     define TLP_QT_SCOPE       __declspec(dllimport)
#     endif
# endif
#endif
#ifndef TLP_QT_SCOPE
#define TLP_QT_SCOPE
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_QT2
#   define TLP_QT_SIMPLE_SCOPE
# else
#     ifdef DLL_TULIP_QT
#     define TLP_QT_SIMPLE_SCOPE  __declspec(dllexport)
#     else
#     define TLP_QT_SIMPLE_SCOPE
#     endif
# endif
#endif
#ifndef TLP_QT_SIMPLE_SCOPE
#define TLP_QT_SIMPLE_SCOPE
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_OGDF
#   define TLP_OGDF_SCOPE         __declspec(dllexport)
#   else
#   define TLP_OGDF_SCOPE       __declspec(dllimport)
#   endif
#endif
#ifndef TLP_OGDF_SCOPE
#define TLP_OGDF_SCOPE
#endif

#endif //TULIPCONF_H
