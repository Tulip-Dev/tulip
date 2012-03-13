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
  *
  */

// This class ensures the release of allocated memory
// when the program ends
class MemoryBlocks {

public :

  ~MemoryBlocks() {
    for (size_t i = 0 ; i < _allocatedMemory.size() ; ++i) {
      free(_allocatedMemory[i]);
    }
  }

  std::vector<void * > _freeObject[MAXNBTHREADS];
  std::vector<void * > _allocatedMemory;
};

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
    t = getObject(THREAD_NUMBER);
    return t;
  }

  inline void operator delete( void *p ) {
    memBlocks._freeObject[THREAD_NUMBER].push_back(p);
  }
private:

  static MemoryBlocks memBlocks;

  static TYPE* getObject(size_t threadId) {
    TYPE *result;

    if (memBlocks._freeObject[threadId].empty()) {
      TYPE * p = (TYPE *)malloc(BUFFOBJ * sizeof(TYPE));
      memBlocks._allocatedMemory.push_back(p);

      for (size_t j=0; j< BUFFOBJ - 1; ++j) {
        memBlocks._freeObject[threadId].push_back((void *)p);
        p += 1;
      }

      result = p;
    }
    else {
      result = (TYPE *)memBlocks._freeObject[threadId].back();
      memBlocks._freeObject[threadId].pop_back();
    }

    return result;
  }
};

template <typename  TYPE >
MemoryBlocks MemoryPool<TYPE>::memBlocks;

}
#endif // MEMORYPOOL_H
