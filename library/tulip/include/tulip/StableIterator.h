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
#ifndef TULIP_STABLEITERATOR_H
#define TULIP_STABLEITERATOR_H
#include <tulip/Iterator.h>
#include <tulip/tulipconf.h>
#include <vector>
#include <stdlib.h>

namespace tlp {
    /**
     * \addtogroup iterators
     */
    /*@{*/
/**
  *\class StableIterator
  *\brief That class enables to store elements of an iterator and to iterate on a copy.
  *
  * That iterator store all elements accessible by the input iterator into an internal data
  * structure (created at the construction) and then only use that structure for the iteration.
  * Order of iteration is the same. If one knows the number of elements that will be iterated
  * one could give give it in paremeters to speed up the duplication process.
  *
  * \warning By default StableIterator manage the destruction of the iterator given in parameter, (ie,
  * delete will be call on the input iterator).
  *
  * That class is really useful when one need to modify the graph during an iteration. For
  * instance the following code remove all nodes that match the function myfunc().
  * Using standard iterators for that operations is not possible since we modify the graph.
  *
  * \code
  * StableIterator<node> it(graph->getNodes());
  * while(it.hasNext()) {
  *  node n = it.next();
  *  if (myfunc(n))
  *     graph->delNode(n);
  * }
  * \endcode
  *
  * \see stableForEach
  */
template<class itType>
struct StableIterator : public Iterator<itType> {
    //=============================
    StableIterator(Iterator<itType> *itIn, size_t nbElements = 0, bool deleteIterator = true) {
        cloneIt.reserve(nbElements);
        for (;itIn->hasNext();) {
            cloneIt.push_back(itIn->next());
        }
        if (deleteIterator)
            delete itIn;
        itStl = cloneIt.begin();
    }
    //=============================
    ~StableIterator(){};
    //=============================
    itType next() {
        itType tmp(*itStl);
        ++itStl;
        return tmp;
    }
    //=============================
    bool hasNext() {
        return (itStl != cloneIt.end());
    };
    //=============================
    /**
    * Enables to restart the iteration at the begining.
    */
    void restart() {
        itStl = cloneIt.begin();
    }
    //=============================
protected :
        std::vector<itType> cloneIt;
        typename std::vector<itType>::const_iterator itStl;
};
/*@}*/
}
#endif
