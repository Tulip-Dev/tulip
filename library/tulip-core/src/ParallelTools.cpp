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

#ifdef TLP_NO_THREADS
// the header below is needed
// to have access to std::ignore
#include <tuple>

#else

#ifdef _OPENMP

#include <omp.h>

#ifdef __APPLE__

#include <tulip/TlpTools.h>

tlp::OpenMPLock::OpenMPLock() : _lock(nullptr) {
  if (!TulipProgramExiting) {
    _lock = new omp_lock_t;
    omp_init_lock(_lock);
  }
}
tlp::OpenMPLock::~OpenMPLock() {
  if (!TulipProgramExiting) {
    omp_destroy_lock(_lock);
  }
  delete _lock;
}
void tlp::OpenMPLock::lock() {
  if (!TulipProgramExiting) {
    omp_set_lock(_lock);
  }
}
void tlp::OpenMPLock::unlock() {
  if (!TulipProgramExiting) {
    omp_unset_lock(_lock);
  }
}

#endif // __APPLE__

struct OpenMPDefaultOptions {
  OpenMPDefaultOptions() {
    int num_threads = omp_get_num_procs();
    auto num_threads_ptr = getenv("OMP_NUM_THREADS");
    if (num_threads_ptr)
      num_threads = atoi(num_threads_ptr);
    omp_set_num_threads(num_threads);

#if _OPENMP > 200805
    // supported since OpenMP 3.0
    omp_set_max_active_levels(2);
#else
#if _OPENMP < 201811
#ifdef _LINUX
    // OMP_NESTED=TRUE
    // must be used with care (especially on MacOSX)
    // because it may lead to really poor performances
    // in comparison with a mono thread execution
    bool nested = true;
#else
    bool nested = false;
#endif
    // deprecated since OpenMP 5.0
    auto nested_ptr = getenv("OMP_NESTED");
    if (nested_ptr)
      // use env variable value (TRUE or FALSE)
      nested = (nested_ptr[0] == 'T');
    omp_set_nested(nested);
#endif
#endif
    bool dynamic = false;
    auto dynamic_ptr = getenv("OMP_DYNAMIC");
    if (dynamic_ptr)
      // use env variable value (TRUE or FALSE)
      dynamic = (dynamic_ptr[0] == 'T');
    omp_set_dynamic(dynamic);
  }
};

static OpenMPDefaultOptions openMpDefaultOptions;

#else

#include <condition_variable>
#include <unordered_map>
#include <tulip/IdManager.h>

#endif

#endif

namespace tlp {

#ifndef TLP_NO_THREADS
#ifdef _OPENMP
unsigned int tlp::ThreadManager::maxNumberOfThreads(omp_get_num_procs());
#else
unsigned int ThreadManager::maxNumberOfThreads(std::thread::hardware_concurrency());
#endif
#else
unsigned int ThreadManager::maxNumberOfThreads(1);
#endif

unsigned int ThreadManager::getNumberOfProcs() {
#ifndef TLP_NO_THREADS
#ifdef _OPENMP
  return omp_get_num_procs();
#else
  return std::thread::hardware_concurrency();
#endif
#else
  return 1;
#endif
}

unsigned int ThreadManager::getNumberOfThreads() {
  return maxNumberOfThreads;
}

void ThreadManager::setNumberOfThreads(unsigned int nbThreads) {
#ifndef TLP_NO_THREADS
  maxNumberOfThreads = std::min(nbThreads, uint(TLP_MAX_NB_THREADS));
#ifdef _OPENMP
  omp_set_num_threads(maxNumberOfThreads);
#endif
#else
  std::ignore = nbThreads;
#endif
}

#if !defined(TLP_NO_THREADS) && !defined(_OPENMP)

// the manager of the thread associated number
// which must be in the range between 0 and maxNumberOfThreads
// 0 is reserved to the main thread
static IdContainer<uint> tNumManager;
// a mutex to ensure serialisation when allocating the thread number
static std::mutex tNumMtx;
// the global map used to register the thread number
static std::unordered_map<std::thread::id, uint> tNumMap;

void ThreadManager::allocateThreadNumber() {
  // exclusive access to tNumManager
  tNumMtx.lock();
  // 0 is reserved for main thread
  auto num = tNumManager.get() + 1;
  auto tNum = std::this_thread::get_id();
  tNumMap[tNum] = num;
  tNumMtx.unlock();
}

void ThreadManager::freeThreadNumber() {
  // exclusive access to tNumManager
  tNumMtx.lock();
  auto tNum = std::this_thread::get_id();
  unsigned int num = tNumMap[tNum];
  tNumMap.erase(tNum);
  assert(num > 0);
  tNumManager.free(num - 1);
  tNumMtx.unlock();
}

#endif

unsigned int ThreadManager::getThreadNumber() {
#ifndef TLP_NO_THREADS
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  tNumMtx.lock();
  auto it = tNumMap.find(std::this_thread::get_id());
  if (it != tNumMap.end()) {
    tNumMtx.unlock();
    return it->second;
  }
  tNumMtx.unlock();
#endif
#endif
  return 0;
}
} // namespace tlp
