#ifndef FILTERITERATOR_H
#define FILTERITERATOR_H

#include <tulip/memorypool.h>



namespace tlp {
    /**
      * \addtogroup iterator
      */
    /*@{*/
    /**
      * @class FilterIterator
      * @ingroup iterators
      * @brief Iterator that enables to filter an other Iterator
      * @param it the iterator that should be filtered
      * @param filter the functor that enables to test wheter or not an element is filtered
      *
      * The functor function shoul have the following form
      * @code
      * class AFilterFunctor {
      *  bool operator()(TYPE a) {
      *    return true if a should be iterated, false if a should be removed;
      *  }
      * };
      * @endcode
      */
    template <typename TYPE, typename FILTER>
    class FilterIterator : public Iterator<TYPE> {
    public:
        FilterIterator(Iterator<TYPE> *it, FILTER filter):
                _it(it),
                _filter(filter) {
            update();
        }
        ~FilterIterator(){
            delete _it;
        }
        inline TYPE next() {
            TYPE tmp = _curVal;
            update();
            return tmp;
        }
        inline bool hasNext() {
            return _hasNext;
        }

    private:
        void update() {
            _hasNext = false;
            while (_it->hasNext()) {
                _curVal = _it->next();
                if (_filter(_curVal)) {
                    _hasNext = true;
                    break;
                }
            }
        }

        bool _hasNext;
        Iterator<TYPE> *_it;
        TYPE   _curVal;
        FILTER _filter;
        size_t _nbele;
    };
    /**
      * @class MPFilterIterator
      * @ingroup iterators
      * @brief MPFilterIterator implements memory pool for FilterIterator
      * @warning never inherit from that class
      * @see FilterIterator
      */
    template <typename TYPE, typename FILTER>
    class MPFilterIterator : public FilterIterator<TYPE, FILTER>, public MemoryPool<MPFilterIterator<TYPE, FILTER> > {
    public:
        MPFilterIterator(Iterator<TYPE> *it, FILTER filter):
                FilterIterator<TYPE, FILTER>(it, filter) {
        }
    };
    /*@}*/
}
#endif // FILTERITERATOR_H
