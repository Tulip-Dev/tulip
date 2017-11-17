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

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef _OPENMP
#define THREAD_NUMBER  omp_get_thread_num()
static const size_t MAXNBTHREADS = 128;
#else
#define THREAD_NUMBER 0
static const size_t MAXNBTHREADS = 1;
#endif

static const size_t BUFFOBJ      = 20;

namespace tlp {
/**
  * @class MemoryPool
  * \brief That class enables to easily create a memory pool for an a class
  *
  * It allocates chunk of BUFFOBJ size of continous memory to allocate
  * instance of the class. After a delete the memory is not free, and
  * will be reused at the next new of the class.
  *
  * @warning it is not recommended to inherit from an object that inherit of that class
  *
  * The following  code only calls malloc one time even if NBTRY object are created
  * in that example, the speedup is about 23, without MemoryPool malloc is called NBTRY times
  * @code
  * class A : public MemoryPool<A> {
  * public:
  *     A(){}
  *    ~A(){}
  *    int data[1000];
  * };
  *
  * size_t NBTRY = 1000 * 1000;
  * for (size_t j=0; j < NBTRY; ++j) {
  *    A *a = new A();
  *    a->data[100] = j;
  *    r1 += a->data[100];
  *    delete a;
  * }
  * @endcode
  *
  */
template <typename  TYPE >
class MemoryPool {
public:
  MemoryPool() {
  }

#ifndef NDEBUG
  inline void *operator new( size_t sizeofObj) {
#else
  inline void *operator new( size_t ) {
#endif
    assert(sizeof(TYPE) == sizeofObj); //to prevent inheritance with different size of object
    TYPE * t;
    t = _memoryChunkManager.getObject(THREAD_NUMBER);
    return t;
  }

  inline void operator delete( void *p ) {
    _memoryChunkManager.releaseObject(THREAD_NUMBER, p);
  }
private:

  class MemoryChunkManager {
  public:

    ~MemoryChunkManager() {
      for (unsigned int i = 0 ; i < MAXNBTHREADS ; ++i) {
        for (size_t j = 0 ; j < _allocatedChunks[i].size() ; ++j) {
          free(_allocatedChunks[i][j]);
        }
      }
    }

    TYPE* getObject(size_t threadId) {
      TYPE *result;

      if (_freeObject[threadId].empty()) {
        void *chunk = malloc(BUFFOBJ * sizeof(TYPE));
        TYPE * p = static_cast<TYPE *>(chunk);
        _allocatedChunks[threadId].push_back(chunk);

        for (size_t j=0; j< BUFFOBJ - 1; ++j) {
          _freeObject[threadId].push_back(static_cast<void *>(p));
          p += 1;
        }

        result = p;
      }
      else {
        result = static_cast<TYPE *>(_freeObject[threadId].back());
        _freeObject[threadId].pop_back();
      }

      return result;
    }

    void releaseObject(size_t threadId, void *p) {
      _freeObject[threadId].push_back(p);
    }

  private:
    std::vector<void * > _allocatedChunks[MAXNBTHREADS];
    std::vector<void * > _freeObject[MAXNBTHREADS];

  };

  static MemoryChunkManager _memoryChunkManager;

};

template <typename  TYPE >
typename MemoryPool<TYPE>::MemoryChunkManager MemoryPool<TYPE>::_memoryChunkManager;

}
#endif // MEMORYPOOL_H
///@endcond
