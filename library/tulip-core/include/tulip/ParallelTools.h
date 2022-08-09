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

#ifndef TLP_PARALLEL_TOOLS_H
#define TLP_PARALLEL_TOOLS_H

#include <tulip/tulipconf.h>
#include <vector>

#ifndef TLP_NO_THREADS

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

#ifdef __APPLE__
struct omp_lock_t;
#endif

#else

// OpenMP no available use C++11 threads
#include <iostream>
#include <algorithm>
#include <mutex>
#include <thread>

#endif

#endif

namespace tlp {

class TlpThread;
// ===================================================================================

/**
 * @brief Static wrapper class around std::thread
 *
 * @since Tulip 5.2
 */
class TLP_SCOPE ThreadManager {

  static unsigned int maxNumberOfThreads;

#if !defined(TLP_NO_THREADS) && !defined(_OPENMP)

  // allocate a number for the calling thread
  static void allocateThreadNumber();

  // deallocate the number of the calling thread
  static void freeThreadNumber();

  // create a thread dedicated to the execution of a function
  // used to iterate between begin and end indices
  template <typename runFunction>
  static inline std::thread *launchThread(const runFunction &f, uint begin, uint end) {
    auto thrdFunction = [&](uint begin, uint end) {
      allocateThreadNumber();
      f(begin, end);
      freeThreadNumber();
    };
    return new std::thread(thrdFunction, begin, end);
  }

#endif

public:
#if !defined(TLP_NO_THREADS) && !defined(_OPENMP)

  // create a thread dedicated to the execution of a function
  // with no arguments
  template <typename runFunction>
  static inline std::thread *launchThread(const runFunction &f) {
    auto thrdFunction = [&]() {
      allocateThreadNumber();
      f();
      freeThreadNumber();
    };
    return new std::thread(thrdFunction);
  }

#endif

  /**
   * Returns the number of processors on the host system.
   */
  static unsigned int getNumberOfProcs();

  /**
   * Returns the number of threads used by default in subsequent OpenMP parallel sections.
   */
  static unsigned int getNumberOfThreads();

  /**
   * Sets the number of threads used by default in subsequent parallel sections.
   */
  static void setNumberOfThreads(unsigned int nbThreads);

  /**
   * Returns the current thread number.
   */
  static unsigned int getThreadNumber();

#ifndef _OPENMP

  /**
   * Parallel iteration of the same function over partitioned ranges
   * between 0 and maxId
   */
  template <typename ThreadFunction>
  static inline void iterate(size_t maxId, const ThreadFunction &threadFunction) {
#ifndef TLP_NO_THREADS
    if (maxId == 0)
      return;

    // compute the size of range indices for each thread
    size_t nbPerThread = maxId == 1 ? 1 : std::max(maxId / (maxNumberOfThreads - 1), size_t(2));
    // begin and end indices of thread partition
    size_t begin = 0, end = nbPerThread;
    maxId -= end - begin;
    // create threads
    std::vector<std::thread *> threads;
    threads.reserve(maxNumberOfThreads - 1);
    while (maxId) {
      threads.push_back(launchThread(threadFunction, begin, end));
      if (nbPerThread > 1) {
        if (maxNumberOfThreads - threads.size() == maxId)
          nbPerThread = 1;
      }
      begin = end;
      end += std::min(nbPerThread, maxId);
      maxId -= end - begin;
    }
    // iterate on last partition
    threadFunction(begin, end);
    // wait for threads
    for (auto thrd : threads) {
      thrd->join();
      delete thrd;
    }
#else
    threadFunction(0, maxId);
#endif
  }

#endif
};

#ifndef TLP_NO_THREADS

#define TLP_MAX_NB_THREADS 128

#define TLP_NB_THREADS tlp::ThreadManager::getNumberOfThreads()

#ifdef _OPENMP

#ifdef __APPLE__

// In order to workaround an odd phenomenon on some MacOS runtimes when an application
// shutdowns, use OpenMP C API to declare critical sections instead of directives.
// https://www.imagemagick.org/discourse-server/viewtopic.php?f=3&t=29031&start=15#p129707
// gives more details about the issue

class OpenMPLock {
public:
  OpenMPLock();
  ~OpenMPLock();
  void lock();
  void unlock();

private:
  omp_lock_t *_lock;
};

#define TLP_LOCK_SECTION(mtx)                                                                      \
  static tlp::OpenMPLock mtx;                                                                      \
  mtx.lock();
#define TLP_UNLOCK_SECTION(mtx) mtx.unlock();
#define TLP_DECLARE_GLOBAL_LOCK(mtx) extern tlp::OpenMPLock mtx
#define TLP_DEFINE_GLOBAL_LOCK(mtx) tlp::OpenMPLock mtx
#define TLP_GLOBALLY_LOCK_SECTION(mtx) mtx.lock();
#define TLP_GLOBALLY_UNLOCK_SECTION(mtx) mtx.unlock()

#else // __APPLE__

#define TLP_LOCK_SECTION(mtx) OMP_CRITICAL_SECTION(mtx)
#define TLP_UNLOCK_SECTION(mtx)
#define TLP_DECLARE_GLOBAL_LOCK(mtx) extern void mtx()
#define TLP_DEFINE_GLOBAL_LOCK(mtx) extern void mtx()
#define TLP_GLOBALLY_LOCK_SECTION(mtx) OMP_CRITICAL_SECTION(mtx)
#define TLP_GLOBALLY_UNLOCK_SECTION(mtx)

#endif // __APPLE__

#else

#define TLP_LOCK_SECTION(mtx)                                                                      \
  static std::mutex mtx;                                                                           \
  mtx.lock();
#define TLP_UNLOCK_SECTION(mtx) mtx.unlock()
#define TLP_DECLARE_GLOBAL_LOCK(mtx) extern std::mutex mtx
#define TLP_DEFINE_GLOBAL_LOCK(mtx) std::mutex mtx
#define TLP_GLOBALLY_LOCK_SECTION(mtx) mtx.lock();
#define TLP_GLOBALLY_UNLOCK_SECTION(mtx) mtx.unlock()

#endif

#else

// no multi-threading
#define TLP_MAX_NB_THREADS 1

#define TLP_NB_THREADS 1

#define TLP_LOCK_SECTION(mtx)
#define TLP_UNLOCK_SECTION(mtx)
#define TLP_DECLARE_GLOBAL_LOCK(mtx) extern void mtx()
#define TLP_DEFINE_GLOBAL_LOCK(mtx) extern void mtx()
#define TLP_GLOBALLY_LOCK_SECTION(mtx)
#define TLP_GLOBALLY_UNLOCK_SECTION(mtx)

#endif

// ===================================================================================

/**
 * Template function to ease the creation of parallel threads taking
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
 * TLP_PARALLEL_MAP_INDICES(N, [&](unsigned int i) {
 *   // run task in a thread
 *   result[i] = computationIntensiveTask(i);
 * });
 * @endcode
 */
template <typename IdxFunction>
void inline TLP_PARALLEL_MAP_INDICES(size_t maxIdx, const IdxFunction &idxFunction) {
#ifdef _OPENMP
  OMP(parallel for)
  for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(maxIdx); ++i) {
    idxFunction(i);
  }
#else
  auto threadFunction = [&](size_t begin, size_t end) {
    for (; begin < end; ++begin) {
      idxFunction(begin);
    }
  };
  ThreadManager::iterate(maxIdx, threadFunction);
#endif
}

template <typename EltType, typename IdxFunction>
void inline TLP_PARALLEL_MAP_VECTOR(const std::vector<EltType> &vect,
                                    const IdxFunction &idxFunction) {
#ifdef _OPENMP
  auto maxIdx = vect.size();
  OMP(parallel for)
  for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(maxIdx); ++i) {
    idxFunction(vect[i]);
  }
#else
  auto threadFunction = [&](size_t begin, size_t end) {
    for (; begin < end; ++begin) {
      idxFunction(vect[begin]);
    }
  };

  ThreadManager::iterate(vect.size(), threadFunction);
#endif
}

template <typename EltType, typename IdxFunction>
void inline TLP_PARALLEL_MAP_VECTOR_AND_INDICES(const std::vector<EltType> &vect,
                                                const IdxFunction &idxFunction) {
#ifdef _OPENMP
  auto maxIdx = vect.size();
  OMP(parallel for)
  for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(maxIdx); ++i) {
    idxFunction(vect[i], i);
  }
#else
  auto threadFunction = [&](size_t begin, size_t end) {
    for (; begin < end; ++begin) {
      idxFunction(vect[begin], begin);
    }
  };

  ThreadManager::iterate(vect.size(), threadFunction);
#endif
}

template <typename F1, typename F2>
void inline TLP_PARALLEL_SECTIONS(const F1 &f1, const F2 &f2) {
#ifndef TLP_NO_THREADS
#ifdef _OPENMP
  OMP(parallel) {
    OMP(sections nowait) {
      OMP(section) {
        f1();
      }
    }
    OMP(master) {
      f2();
    }
  }
#else
  auto thrd = ThreadManager::launchThread(f1);
  f2();
  thrd->join();
  delete thrd;
#endif
#else
  f1();
  f2();
#endif
}

template <typename F1, typename F2, typename F3>
void inline TLP_PARALLEL_SECTIONS(const F1 &f1, const F2 &f2, const F3 &f3) {
#ifndef TLP_NO_THREADS
#ifdef _OPENMP
  OMP(parallel) {
    OMP(sections nowait) {
      OMP(section) {
        f1();
      }
      OMP(section) {
        f2();
      }
    }
    OMP(master) {
      f3();
    }
  }
#else
  auto thrd1 = ThreadManager::launchThread(f1);
  auto thrd2 = ThreadManager::launchThread(f2);
  f3();
  thrd1->join();
  thrd2->join();
  delete thrd1;
  delete thrd2;
#endif
#else
  f1();
  f2();
  f3();
#endif
}

template <typename F1, typename F2, typename F3, typename F4>
void inline TLP_PARALLEL_SECTIONS(const F1 &f1, const F2 &f2, const F3 &f3, const F4 &f4) {
#ifndef TLP_NO_THREADS
#ifdef _OPENMP
  OMP(parallel) {
    OMP(sections nowait) {
      OMP(section) {
        f1();
      }
      OMP(section) {
        f2();
      }
      OMP(section) {
        f3();
      }
    }
    OMP(master) {
      f4();
    }
  }
#else
  auto thrd1 = ThreadManager::launchThread(f1);
  auto thrd2 = ThreadManager::launchThread(f2);
  auto thrd3 = ThreadManager::launchThread(f3);
  f4();
  thrd1->join();
  thrd2->join();
  thrd3->join();
  delete thrd1;
  delete thrd2;
  delete thrd3;
#endif
#else
  f1();
  f2();
  f3();
  f4();
#endif
}
} // namespace tlp

#endif // TLP_PARALLEL_TOOLS_H

///@endcond
