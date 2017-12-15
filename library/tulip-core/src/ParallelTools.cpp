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

#include <tulip/ParallelTools.h>

#include <algorithm>
#include <iostream>
#include <tuple>

#ifdef _OPENMP
#include <omp.h>

struct OpenMPDefaultOptions {
  OpenMPDefaultOptions() {
    omp_set_num_threads(omp_get_num_procs());
    omp_set_nested(true);
    omp_set_dynamic(false);
  }
};

static OpenMPDefaultOptions openMpDefaultOptions;

#endif

#ifdef _OPENMP
unsigned int tlp::OpenMPManager::maxNumberOfThreads(omp_get_num_procs());
#else
unsigned int tlp::OpenMPManager::maxNumberOfThreads(1);
#endif

unsigned int tlp::OpenMPManager::getNumberOfProcs() {
#ifdef _OPENMP
  return omp_get_num_procs();
#else
  return 1;
#endif
}

unsigned int tlp::OpenMPManager::getNumberOfThreads() {
#ifdef _OPENMP
  return maxNumberOfThreads;
#else
  return 1;
#endif
}

void tlp::OpenMPManager::setNumberOfThreads(unsigned int nbThreads) {
#ifdef _OPENMP
  maxNumberOfThreads = std::min(nbThreads, uint(TLP_MAX_NB_THREADS));
  omp_set_num_threads(maxNumberOfThreads);
#else
  std::ignore = nbThreads;
#endif
}

unsigned int tlp::OpenMPManager::getThreadNumber() {
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

void tlp::OpenMPManager::setNested(bool nested) {
#ifdef _OPENMP
  omp_set_nested(nested);
#else
  std::ignore = nested;
#endif
}

bool tlp::OpenMPManager::nested() {
#ifdef _OPENMP
  return omp_get_nested();
#else
  return false;
#endif
}

void tlp::OpenMPManager::setDynamic(bool dynamic) {
#ifdef _OPENMP
  omp_set_dynamic(dynamic);
#else
  std::ignore = dynamic;
#endif
}

bool tlp::OpenMPManager::dynamic() {
#ifdef _OPENMP
  return omp_get_dynamic();
#else
  return false;
#endif
}
