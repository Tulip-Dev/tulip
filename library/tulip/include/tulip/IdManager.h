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
#ifndef DOXYGEN_NOTFOR_DEVEL

#ifndef _TULIPIDMANAGER_H
#define _TULIPIDMANAGER_H
#include <set>
#include <iostream>
#include "tulip/Iterator.h"

namespace tlp {

// define a minimal structure to hold the infos manage by an id manager
struct IdManagerInfos {
  // the first id in use
  unsigned int firstId;
  // the next id to use
  unsigned int nextId;
  // the unused ids between the first and the next
  std::set<unsigned int> freeIds;

  IdManagerInfos(): firstId(0), nextId(0) {}
};
  
// define a class to iterate through the allocated ids
template <typename TYPE>
class IdManagerIterator:public Iterator<TYPE> {

private:
  unsigned int current;
  unsigned int last;
  const std::set<unsigned int>& freeIds; 
  std::set<unsigned int>::const_iterator it;

 public:
  
 IdManagerIterator(const IdManagerInfos& infos):
  current(infos.firstId), last(infos.nextId), freeIds(infos.freeIds), it(freeIds.begin()) {
    std::set<unsigned int>::const_reverse_iterator itr;
    itr = freeIds.rbegin();
    while(itr != freeIds.rend() && (*itr) == last - 1) {
      --last;
      ++itr;
    }
  }
  ~IdManagerIterator() {}

  bool hasNext() {
  return (current < last);
  }

  TYPE next() {
    unsigned int tmp = current;
    ++current;
    while(it != freeIds.end()) {
      if (current < *it) return (TYPE) tmp;
      ++current;
      ++it;
    }
    return (TYPE) tmp;
  }
};

/// class for the management of the identifiers : node, edge
class TLP_SCOPE IdManager: public IdManagerInfos {

public:
 IdManager() {}
  /**
   * Returns true if the id is not used else false.
   */
  bool is_free(unsigned int) const;
  /**
   * Free the id given in parameter.
   */
  void free(const unsigned int);
  /**
   * Returns a new id, the returned id is choosen to minimize
   * the memory space of the free list, and the fragmentation
   * of the ids.
   */
  unsigned int get();
  /**
   * assuming the given id is free.
   * remove it from free ids
   * (used to ensure the same id when loading a graph with subgraphs)
   */
  void getFreeId(unsigned int id);
  /**
   * indicates if space of ids is fragmented
   *
   */
  bool hasFreeIds() const;
  /**
   * returns first used id
   */
  unsigned int getFirstId() const {
    return firstId;
  }
  /**
   * restore infos, used by push/pop
   */
  void restoreInfos(const IdManagerInfos& infos) {
    firstId = infos.firstId;
    nextId = infos.nextId;
    freeIds = infos.freeIds;
  }

  /**
   * Returns an iterator on all the used ids. Warning, if
   * the idManager is modified (free, get) this iterator 
   * will be invalid.
   */
  template<typename TYPE>
    Iterator<TYPE>* getIds() const {
    return new IdManagerIterator<TYPE>(*this);
  }

  friend std::ostream& operator<<(std::ostream &, const IdManager &);
  friend class IdManagerTest;

};
std::ostream& operator<<(std::ostream &,const IdManager &);

}

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
