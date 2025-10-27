/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#define _DEPRECATED __attribute__((deprecated))
#define _DEPRECATED_TYPEDEF(type, deprecated_type) typedef type deprecated_type _DEPRECATED

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
