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

#ifndef DOXYGEN_NOTFOR_DEVEL

#ifndef _TULIPIDMANAGER_H
#define _TULIPIDMANAGER_H
#include <set>
#include <iostream>
#include <tulip/Iterator.h>

namespace tlp {

// define a minimal structure to hold the state of an id manager
struct IdManagerState {
  // the first id in use
  unsigned int firstId;
  // the next id to use
  unsigned int nextId;
  // the unused ids between the first and the next
  std::set<unsigned int> freeIds;

  IdManagerState(): firstId(0), nextId(0) {}
};

// define a class to iterate through the non free ids
// of an IdManagerState
template <typename TYPE>
class IdManagerIterator:public Iterator<TYPE> {

private:
  unsigned int current;
  unsigned int last;
  const std::set<unsigned int>& freeIds;
  std::set<unsigned int>::const_iterator it;

public:

  IdManagerIterator(const IdManagerState& infos):
    current(infos.firstId), last(infos.nextId), freeIds(infos.freeIds), it(freeIds.begin()) {
#ifdef TLP_NO_IDS_REUSE
    std::set<unsigned int>::const_reverse_iterator itr;
    itr = freeIds.rbegin();

    while(itr != freeIds.rend() && (*itr) == last - 1) {
      --last;
      ++itr;
    }

#endif
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
class TLP_SCOPE IdManager {

  // the current state
  IdManagerState state;

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
   * Returns a new id.
   */
  unsigned int get() {
#ifdef TLP_NO_IDS_REUSE
    return state.nextId++;
#else
    return state.freeIds.empty() ? state.nextId++ : getFreeId();
#endif
  }
  /**
   * Returns the first id of a range of nb new ids.
   */
  unsigned int getFirstOfRange(unsigned nb) {
    unsigned int first = state.nextId;
    state.nextId += nb;
    return first;
  }

#ifndef TLP_NO_IDS_REUSE
  /**
   * remove and return the first available id from the free ids
   */
  unsigned int getFreeId();
#endif
  /**
   * assuming the given id is free.
   * remove it from free ids
   * (used to ensure the same id when loading a graph with subgraphs)
   */
  void getFreeId(unsigned int id);
  /**
   * return the current state of the Id manager
   */
  const IdManagerState& getState() {
    return state;
  }
  /**
   * restore a saved state, used by push/pop
   */
  void restoreState(const IdManagerState& infos) {
    state = infos;
  }
  /**
   * Returns an iterator on all the used ids. Warning, if
   * the idManager is modified (free, get) this iterator
   * will be invalid.
   */
  template<typename TYPE>
  Iterator<TYPE>* getIds() const {
    return new IdManagerIterator<TYPE>(state);
  }

  friend std::ostream& operator<<(std::ostream &, const IdManager &);
  friend class IdManagerTest;
};

std::ostream& operator<<(std::ostream &,const IdManager &);

}

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
