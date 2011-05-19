#ifndef CONVERSIONITERATOR_H
#define CONVERSIONITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/memorypool.h>
//
namespace tlp {
    /**
      * \addtogroup iterator
      */
    /*@{*/
    /**
      * @class ConversionIterator
      * @ingroup Iterators
      * @brief Iterator that enables to convert an Iterator of type TYPEIN to an Iterator if type TYPEOUT
      * @param it, the iterator that should be converted
      * @param convFunctor, the functor that enable to convert TYPEIN in TYPEOUT
      *
      * The functor function shoul have the following form
      * @code
      * class AConversionFunctor {
      *  TYPEOUT operator()(TYPEIN a) {
      *    return a converted in TYPEOUT;
      *  }
      * };
      * @endcode
      */
    template <typename TYPEIN, typename TYPEOUT, typename CONVERSIONFUNCTOR>
    class ConversionIterator : public Iterator<TYPEOUT> {
    public:
        ConversionIterator(Iterator<TYPEIN> *it, CONVERSIONFUNCTOR convFunctor):_it(it), _convFunctor(convFunctor){
        }
        ~ConversionIterator() {
            delete _it;
        }
        inline bool hasNext() {
            return _it->hasNext();
        }
        inline TYPEOUT next() {
            return _convFunctor(_it->next());
        }
    private:
        tlp::Iterator<TYPEIN> *_it;
        CONVERSIONFUNCTOR _convFunctor;
    };
    /**
      * @class MPConversionIterator
      * @ingroup Iterators
      * @brief MPConversionIterator implements memory pool for ConversionIterator
      * @warning never inherit from that class
      * @see ConversionIterator
      */
    template <typename TYPEIN, typename TYPEOUT, typename CONVERSIONFUNCTOR>
    class MPConversionIterator : public ConversionIterator<TYPEIN, TYPEOUT, CONVERSIONFUNCTOR>, public MemoryPool<MPConversionIterator<TYPEIN, TYPEOUT, CONVERSIONFUNCTOR> > {
    public:
        MPConversionIterator(Iterator<TYPEIN> *it, CONVERSIONFUNCTOR convFunctor):
                ConversionIterator<TYPEIN, TYPEOUT, CONVERSIONFUNCTOR>(it, convFunctor) {
        }
    };
    /*@}*/
}
#endif // CONVERSIONITERATOR_H
