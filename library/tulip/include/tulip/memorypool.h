#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <vector>
#include <iostream>
#include <cstdlib>
#include <omp.h>

static const size_t BUFFOBJ      = 20;
static const size_t MAXNBTHREADS = 128;

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
            t = getObject(omp_get_thread_num());
            return t;
        }

        inline void operator delete( void *p ) {
            _freeObject[omp_get_thread_num()].push_back(p);
        }
private:
        static std::vector<void * > _freeObject[MAXNBTHREADS];

        static TYPE* getObject(size_t threadId) {
            TYPE *result;
            if (_freeObject[threadId].empty()) {
                TYPE * p = (TYPE *)malloc(BUFFOBJ * sizeof(TYPE));
                for (size_t j=0; j< BUFFOBJ - 1; ++j) {
                    _freeObject[threadId].push_back((void *)p);
                    p += 1;
                }
                result = p;
            }
            else {
                result = (TYPE *)_freeObject[threadId].back();
                _freeObject[threadId].pop_back();
            }
            return result;
        }
};

template <typename  TYPE >
std::vector<void * > MemoryPool<TYPE>::_freeObject[MAXNBTHREADS];

}
#endif // MEMORYPOOL_H
