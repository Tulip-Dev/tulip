#ifndef POOLEDSTLITERATOR_H
#define POOLEDSTLITERATOR_H

#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/memorypool.h>


namespace tlp {
    /**
      * \addtogroup iterator
      */
    /*@{*/
    /**
      * @class StlFilterIterator
      * @brief Iterator that enables to filter a Stl Iterator
      * @param startIt the beginning of the iterator that should be filtered (begin())
      * @param endIt   the end of the iterator that should be filtered (begin())
      * @param filter  the functor that enables to test wheter or not an element is filtered
      *
      * That example enable to iterate only of elements greater than 50;
      * @code
      * class GreaterFilter {
      *  GreaterFilter(double threshold):_t(threshold){
      *  }
      *  bool operator()(double a) {
      *    return a < _t;
      *  }
      * };
      * int main() {
      * vector<double> vec(100);
      * for (size_t j=0; j<100; ++j)
      *    vec[j] = j;
      * GreaterFilter filter(50);
      * double x;
      * forEach(x, new StlFilterIterator<double, vector<double>::const_iterator>(vec.begin(), vec.end(), filter)) {
      *   cout << x << endl;
      * }
      * @endcode
      */
    template<typename VALUE, typename ITERATOR, typename FILTER>
    class StlFilterIterator : public Iterator< VALUE  > {
    public:
      StlFilterIterator(const ITERATOR &startIt, const ITERATOR &endIt,  FILTER filter):
        it(startIt),
        itEnd(endIt),
        _filter(filter) {
          update();
      }

      VALUE next() {
        VALUE tmp = *it;
        ++it;
        update();
        return tmp;
      }

      bool hasNext() {
        return (itEnd!=it);
      }

  private:

      void update() {
          while( it != itEnd && !_filter(*it))
              ++it;
      }

      ITERATOR it, itEnd;
      FILTER &_filter;
  };

    /**
      * @class MPStlFilterIterator
      * @ingroup Iterators
      * @brief MPStlFilterIterator implements memory pool for StlFilterIterator
      * @warning never inherit from that class
      * @see StlFilterIterator
      */
    template<typename VALUE, typename ITERATOR, typename FILTER>
    struct MPStlFilterIterator:public StlFilterIterator< VALUE, ITERATOR, FILTER >,
                             public MemoryPool<MPStlFilterIterator<VALUE, ITERATOR, FILTER> > {
      MPStlFilterIterator(const ITERATOR &startIt, const ITERATOR &endIt, FILTER filter):
              StlFilterIterator<VALUE, ITERATOR, FILTER>(startIt, endIt, filter) {
      }

    };


    /*@}*/
  };
}
#endif // POOLEDSTLITERATOR_H
