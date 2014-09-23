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

#ifndef UNIQUEITERATOR_H
#define UNIQUEITERATOR_H
#include <set>
#include <tulip/memorypool.h>


//===============================================================================
namespace tlp {
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
  ~UniqueIterator() {
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

}
#endif // UNIQUEITERATOR_H
///@endcond
