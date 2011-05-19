#ifndef UNIQUEITERATOR_H
#define UNIQUEITERATOR_H
#include <set>
#include <tulip/memorypool.h>


//===============================================================================
namespace tlp {
    /**
      * \addtogroup iterator
      */
    /*@{*/
    /**
      * \brief that class implement a default functor that always return true
      *
      * @see UniqueIterator
      */
    template <typename TYPE>
    class CheckAllFunctor {
        bool operator()(const TYPE &) {
            return true;
        }
    };

    /**
      * @class UniqueIterator
      * @ingroup Iterators
      *
      * @brief UniqueIterator enables to remove duplicated elements in an iterator
      *
      * @param it the iterator in which we want to filter out duplicated elements
      * @param checkFuncor a functor that enable to indicate wheter or not the element could be duplicated (default test all elements)
      *
      * The functor function shoul have the following form
      * @code
      * class ACheckFunctor {
      *  bool operator(TYPE a) {
      *   return true if a could be duplicated else false;
      *  }
      * };
      * @endcode
      * checkFunctor are used for optimization purpose to prevent to log(n) test for all elements when not necessary.
      */
    template <typename TYPE, typename TOCHECKFUNCTOR = CheckAllFunctor<TYPE> >
    class UniqueIterator : public Iterator<TYPE> {
    public:
        UniqueIterator(Iterator<TYPE> *it, TOCHECKFUNCTOR checkFunctor = TOCHECKFUNCTOR()):_it(it), _checkFunctor(checkFunctor) {
           update();
        }
        //================================================
        ~UniqueIterator(){
            delete _it;
        }
        //================================================
        TYPE next() {
            TYPE tmp = curVal;
            update();
            return tmp;
        }
        //================================================
        inline bool hasNext() {
            return _hasNext;
        }
        //================================================
        void update() {
            _hasNext = false;
            while(_it->hasNext()) {
                curVal = _it->next();
                if (_checkFunctor(curVal) ) {
                    if (_flag.find(curVal) == _flag.end()) {
                        _hasNext = true;
                        _flag.insert(curVal);
                        return;
                    }
                }
                else {
                    _hasNext = true;
                    return;
                }
            }
        }

    private:
        std::set<TYPE> _flag;
        bool _hasNext;
        Iterator<TYPE> *_it;
        TYPE curVal;
        TOCHECKFUNCTOR _checkFunctor;
    };
    /**
      * @class MPUniqueIterator
      * @ingroup Iterators
      * @brief MPUniqueIterator implements memory pool for UniqueIterator
      * @warning never inherit from that class
      * @see UniqueIterator
      */
    template <typename TYPE, typename TOCHECKFUNCTOR = CheckAllFunctor<TYPE> >
    class MPUniqueIterator : public UniqueIterator<TYPE, TOCHECKFUNCTOR> ,
                             public MemoryPool<MPUniqueIterator<TYPE, TOCHECKFUNCTOR> > {
    public:
        MPUniqueIterator(Iterator<TYPE> *it, TOCHECKFUNCTOR checkFunctor = TOCHECKFUNCTOR()):
                UniqueIterator<TYPE, TOCHECKFUNCTOR>(it, checkFunctor) {
        }
      };
    /*@}*/
}
#endif // UNIQUEITERATOR_H
