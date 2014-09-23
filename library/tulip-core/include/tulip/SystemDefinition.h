/*
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
///@cond DOXYGEN_HIDDEN

#ifndef SYSTEMDEFINITION_H
#define SYSTEMDEFINITION_H

#if defined(_WIN32)
#define OS_PLATFORM "win"
#elif defined(__APPLE__)
#define OS_PLATFORM "mac"
#elif defined(__linux__)
#define OS_PLATFORM "linux"
#else
#define OS_PLATFORM "other"
#endif

#if defined(__x86_64__) || defined(_M_X64)
#define OS_ARCHITECTURE "x86_64"
#elif defined(__i386__) || defined(_M_IX86)
#define OS_ARCHITECTURE "x86"
#else
#define OS_ARCHITECTURE "other"
#endif

#if defined(__clang__)
#define OS_COMPILER "Clang"
#elif defined(__GNUC__)
#define OS_COMPILER "gcc"
#elif defined(_MSC_VER)
#define OS_COMPILER "MSVC"
#endif

#endif // SYSTEMDEFINITION_H
///@endcond
