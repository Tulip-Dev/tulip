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

#ifndef PARALLEL_TOOLS_H
#define PARALLEL_TOOLS_H

#include <tulip/tulipconf.h>

#ifdef _OPENMP
// _OPENMP is supposed to be defined as an integer
//  representing the year/month of the supported version
#if _OPENMP < 200805
// only signed integer types are supported
// for OpenMP < 3.0
typedef long long OMP_ITER_TYPE;
#else
typedef size_t OMP_ITER_TYPE;
#endif
#ifndef _MSC_VER
#define OMP(x) _Pragma(STRINGIFY(omp x))
#define OMP_CRITICAL_SECTION(x) _Pragma(STRINGIFY(omp critical(x)))
#else
#define OMP(x) __pragma(omp x)
#define OMP_CRITICAL_SECTION(x) __pragma(omp critical(x))
#endif
#define TLP_MAX_NB_THREADS 128
#else
#define OMP(x)
#define OMP_CRITICAL_SECTION(x)
#define TLP_MAX_NB_THREADS 1
#endif

namespace tlp {

// ===================================================================================

/**
 * @brief Static wrapper class around the OpenMP runtime library
 *
 * @since Tulip 5.2
 */
class TLP_SCOPE OpenMPManager {

public:
  /**
   * Returns the number of processors on the host system.
   */
  static unsigned int getNumberOfProcs();

  /**
   * Returns the number of threads used by default in subsequent OpenMP parallel sections.
   */
  static unsigned int getNumberOfThreads();

  /**
   * Sets the number of threads used by default in subsequent OpenMP parallel sections.
   */
  static void setNumberOfThreads(unsigned int nbThreads);

  /**
   * Returns the current OpenMP thread number.
   */
  static unsigned int getThreadNumber();

  /**
   * Enables or disables nested OpenMP parallel regions.
   */
  static void setNested(bool nested);

  /**
   * Returns the nested state.
   */
  static bool nested();

  /**
   * Enables or disables the dynamic adjustment of the number of OpenMP threads.
   */
  static void setDynamic(bool dynamic);

  /**
   * Returns the dynamic state.
   */
  static bool dynamic();

private:
  static unsigned int maxNumberOfThreads;
};
}

// ===================================================================================

/**
 * Template function to ease the creation of OpenMP parallel jobs taking
 * an index as parameter (0 <= index < maxIdx).
 *
 * @since Tulip 5.2
 *
 * @param maxIdx the upper bound exclusive of the indices range
 * @param idxFunction callable object (e.g. lambda function) taking an unsigned integer as parameter
 *
 * Example of use:
 *
 * @code
 * auto computationIntensiveTask = [&](unsigned int id) {
 *  double result = 0;
 *   ...
 *  return result;
 * };
 * const unsigned int N = ... ;
 * std::vector<double> result(N);
 * OMP_PARALLEL_MAP_INDICES(N, [&](unsigned int i) {
 *   // run task in a thread
 *   result[i] = computationIntensiveTask(i);
 * });
 * @endcode
 */
template <typename IdxFunction>
void inline OMP_PARALLEL_MAP_INDICES(unsigned int maxIdx, IdxFunction &&idxFunction) {
#ifdef _OPENMP
  OMP(parallel for)
  for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(maxIdx); ++i) {
    idxFunction(i);
  }
#else
  // fallback to sequential processing if OpenMP is not available (e.g. with Apple Clang)
  for (unsigned int i = 0; i < maxIdx; ++i) {
    idxFunction(i);
  }
#endif
}

#endif
