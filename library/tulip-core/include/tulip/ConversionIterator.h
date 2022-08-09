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

#ifndef CONVERSIONITERATOR_H
#define CONVERSIONITERATOR_H

#include <tulip/Iterator.h>
#include <tulip/StlIterator.h>
#include <tulip/memorypool.h>

namespace tlp {
/**
 * @class ConversionIterator
 * @ingroup Iterators
 * @brief Iterator that enables to convert an Iterator of type TYPEIN to an Iterator of type TYPEOUT
 * @param it the iterator that should be converted
 * @param convFunctor the functor or lamnda function that enables to convert TYPEIN to TYPEOUT
 *
 * The functor function should have the following form:
 * @code
 * class AConversionFunc {
 *  TYPEOUT operator()(TYPEIN a) {
 *    return a converted in TYPEOUT;
 *  }
 * };
 * @endcode
 **/
template <typename TYPEIN, typename TYPEOUT, typename ConversionFunc>
class ConversionIterator : public Iterator<TYPEOUT> {
public:
  ConversionIterator(Iterator<TYPEIN> *it, ConversionFunc convFunctor)
      : _it(it), _convFunctor(convFunctor) {}
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
  ConversionFunc _convFunctor;
};
/**
 * @class MPConversionIterator
 * @ingroup Iterators
 * @brief MPConversionIterator implements memory pool for ConversionIterator
 * @warning never inherit from that class
 * @see ConversionIterator
 **/
template <typename TIN, typename TOUT, typename ConversionFunc>
class MPConversionIterator : public ConversionIterator<TIN, TOUT, ConversionFunc>,
                             public MemoryPool<MPConversionIterator<TIN, TOUT, ConversionFunc>> {
public:
  MPConversionIterator(Iterator<TIN> *it, ConversionFunc convFunctor)
      : ConversionIterator<TIN, TOUT, ConversionFunc>(it, convFunctor) {}
};

/**
 * @brief Convenient function for creating a ConversionIterator.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a ConversionIterator from another Iterator and a conversion function.
 * The returned Iterator takes ownership of the one provided as parameter.
 *
 * @param it a Tulip Iterator
 * @param convFunc functor or lambda function converting a value of type TYPEIN to type TYPEOUT
 *
 * @return a ConversionIterator
 *
 **/
template <typename TOUT, typename ConversionFunc, typename TIN>
inline ConversionIterator<TIN, TOUT, ConversionFunc> *conversionIterator(tlp::Iterator<TIN> *it,
                                                                         ConversionFunc convFunc) {
  return new MPConversionIterator<TIN, TOUT, ConversionFunc>(it, convFunc);
}

/**
 * @brief Convenient function for creating a ConversionIterator from a STL container.
 * @ingroup Iterators
 *
 * @since Tulip 5.2
 *
 * Creates a ConversionIterator from a STL container (std::list, std::vector, std::set, std::map,
 *...)
 * and a conversion function.
 *
 * @param stlContainer any STL container
 * @param convFunc functor or lambda function converting a value from the container to type TYPEOUT
 *
 * @return a ConversionIterator
 *
 **/
template <typename TOUT, typename ConversionFunc, typename Container>
typename std::enable_if<has_const_iterator<Container>::value,
                        ConversionIterator<typename Container::value_type, TOUT, ConversionFunc>
                            *>::type inline conversionIterator(const Container &stlContainer,
                                                               ConversionFunc convFunc) {
  return new MPConversionIterator<typename Container::value_type, TOUT, ConversionFunc>(
      stlIterator(stlContainer), convFunc);
}
} // namespace tlp
#endif // CONVERSIONITERATOR_H
