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
#if !_MSC_VER
#  define TYPEOF typeof
#endif

#if _MSC_VER
//MSVC 2010 has a different keyword for typeof, and deprecated
#  if _MSC_VER >= 1600
#    define _DEPRECATED __declspec(deprecated)
#    define TYPEOF decltype
#  else
//MSVC 2008 must use Boost's typeof
#    include "boost/typeof/typeof.hpp"
#    define TYPEOF BOOST_TYPEOF
#  endif

  //MSVC needs explicit casting of ints ot double, float or long double. Let's just pretend he does not.
  #include <math.h>
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

#  define __PRETTY_FUNCTION__ __FUNCTION__ //MSVC has a different name for pretty_function
#  define strcasecmp stricmp  //strcasecmp does not exists for VC, workaround
#  define cbrt(arg) pow((double)arg, 1.0/3) //VC does not have cbrt, little workaround
#  define isnan(x) ((x) != (x)) //you guessed it, this is a C99 feature, and VC++ does not support C99. workaroud this.
#  define rint(arg) arg > 0 ? (int)std::floor((double)arg) : (int)std::ceil((double)arg) //Hey, nother C99 feature !

//for GCC 3.X
#elif (__GNUC__ < 3)
#  define stdext std
#  define _DEPRECATED
#  include <stl_hash_fun.h>

//clang does not define __GNUC_MINOR__, thus having a separate clang #elif seems cleaner than adding defined() in the #else
#elif __clang__
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
# 	ifdef DLL_TULIP
#		define TLP_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_SCOPE
#define TLP_SCOPE 
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_GL
#		define TLP_GL_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_GL_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_GL_SCOPE
#define TLP_GL_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_QT2
#		define TLP_QT_SCOPE       __declspec(dllimport)
#	else
# 		ifdef DLL_TULIP_QT
#			define TLP_QT_SCOPE         __declspec(dllexport)
# 		else
#			define TLP_QT_SCOPE       __declspec(dllimport)
# 		endif
#	endif
#endif
#ifndef TLP_QT_SCOPE
#define TLP_QT_SCOPE           
#endif

#ifdef _WIN32
#   ifdef DLL_TULIP_QT2
#		define TLP_QT_SIMPLE_SCOPE 
#	else 
# 		ifdef DLL_TULIP_QT
#			define TLP_QT_SIMPLE_SCOPE  __declspec(dllexport)
# 		else
#			define TLP_QT_SIMPLE_SCOPE       
# 		endif
#	endif
#endif
#ifndef TLP_QT_SIMPLE_SCOPE
#define TLP_QT_SIMPLE_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_PLUGINSMANAGER
#		define TLP_PLUGINSMANAGER_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_PLUGINSMANAGER_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_PLUGINSMANAGER_SCOPE
#define TLP_PLUGINSMANAGER_SCOPE           
#endif

#ifdef _WIN32
# 	ifdef DLL_TULIP_OGDF
#		define TLP_OGDF_SCOPE         __declspec(dllexport)
# 	else
#		define TLP_OGDF_SCOPE       __declspec(dllimport)
# 	endif
#endif
#ifndef TLP_OGDF_SCOPE
#define TLP_OGDF_SCOPE           
#endif

#endif //TULIPCONF_H
