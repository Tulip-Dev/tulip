/*
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef CONVERSIONITERATOR_H
#define CONVERSIONITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/memorypool.h>
//
namespace tlp {
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
  ConversionIterator(Iterator<TYPEIN> *it, CONVERSIONFUNCTOR convFunctor):_it(it), _convFunctor(convFunctor) {
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

}
#endif // CONVERSIONITERATOR_H
///@endcond
