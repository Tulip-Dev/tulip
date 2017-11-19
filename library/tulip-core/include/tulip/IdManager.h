/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef _TULIPIDMANAGER_H
#define _TULIPIDMANAGER_H

#ifdef _OPENMP
#include <omp.h>
#endif
#include <algorithm>
#include <climits>
#include <set>
#include <vector>
#include <iostream>
#include <tulip/MutableContainer.h>
#include <tulip/StlIterator.h>

namespace tlp {

// define a minimal structure to hold the state of an id manager
struct IdManagerState {
  // the first id in use
  unsigned int firstId;
  // the next id to use
  unsigned int nextId;
  // the unused ids between the first and the next
  std::set<unsigned int> freeIds;

  IdManagerState() : firstId(0), nextId(0) {}
};

// define a class to iterate through the non free ids
// of an IdManagerState
template <typename TYPE>
class IdManagerIterator : public Iterator<TYPE> {

private:
  unsigned int current;
  unsigned int last;
  const std::set<unsigned int> &freeIds;
  std::set<unsigned int>::const_iterator it;

public:
  IdManagerIterator(const IdManagerState &info)
      : current(info.firstId), last(info.nextId), freeIds(info.freeIds), it(freeIds.begin()) {
#ifdef TLP_NO_IDS_REUSE
    std::set<unsigned int>::const_reverse_iterator itr;
    itr = freeIds.rbegin();

    while (itr != freeIds.rend() && (*itr) == last - 1) {
      --last;
      ++itr;
    }

#endif
  }
  ~IdManagerIterator() override {}

  bool hasNext() override {
    return (current < last);
  }

  TYPE next() override {
    unsigned int tmp = current;
    ++current;

    while (it != freeIds.end()) {
      if (current < *it)
        return static_cast<TYPE>(tmp);

      ++current;
      ++it;
    }

    return static_cast<TYPE>(tmp);
  }
};

/// class for the management of the identifiers
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
    return state.firstId ? --state.firstId : (state.freeIds.empty() ? state.nextId++ : getFreeId());
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
  const IdManagerState &getState() {
    return state;
  }
  /**
   * restore a saved state, used by push/pop
   */
  void restoreState(const IdManagerState &info) {
    state = info;
  }
  /**
   * Returns an iterator on all the used ids. Warning, if
   * the idManager is modified (free, get) this iterator
   * will be invalid.
   */
  template <typename TYPE>
  Iterator<TYPE> *getIds() const {
    return new IdManagerIterator<TYPE>(state);
  }

  friend std::ostream &operator<<(std::ostream &, const IdManager &);
  friend class IdManagerTest;
};

std::ostream &operator<<(std::ostream &, const IdManager &);

// class for the management of the identifiers: node, edge
// or any type which can be easily cast in an unsigned int
template <typename ID_TYPE>
class TLP_SCOPE IdContainer : public std::vector<ID_TYPE> {
  // the number of free ids
  unsigned int nbFree;
  // the position of the ids
  std::vector<unsigned int> pos;

  inline ID_TYPE *&beginPtr() {
    return reinterpret_cast<ID_TYPE **>(this)[0];
  }

  inline ID_TYPE *&sizePtr() {
    return reinterpret_cast<ID_TYPE **>(this)[1];
  }

  inline void setSize(unsigned int size) {
    sizePtr() = beginPtr() + size;
  }

public:
  IdContainer() : std::vector<ID_TYPE>(), nbFree(0) {}

  // reset
  void clear() {
    std::vector<ID_TYPE>::clear();
    pos.clear();
    nbFree = 0;
  }

  // reserve enough room to store nb elts
  void reserve(size_t nb) {
    std::vector<ID_TYPE>::reserve(nb);
    pos.reserve(nb);
  }

  // return whether there is free ids or not
  inline bool hasFree() const {
    return nbFree > 0;
  }

  // return the number of elts in the free storage
  inline unsigned int numberOfFree() const {
    return nbFree;
  }

  // return whether the id exist or not
  inline bool isElement(ID_TYPE elt) const {
    unsigned int id = elt;
    return id < pos.size() && pos[id] != UINT_MAX;
  }

  // return an iterator on the existing elts
  inline Iterator<ID_TYPE> *getElts() const {
    return new StlIterator<ID_TYPE, typename std::vector<ID_TYPE>::const_iterator>(this->begin(),
                                                                                   this->end());
  }

  // return the position of an existing elt
  inline unsigned int getPos(ID_TYPE elt) const {
    assert(isElement(elt));
    return pos[elt];
  }

  // return a new elt
  ID_TYPE get() {
    unsigned int freePos = this->size();

    if (nbFree) {
      setSize(freePos + 1);
      --nbFree;
    } else {
      this->resize(freePos + 1);
      pos.resize(freePos + 1);
      (*this)[freePos] = ID_TYPE(freePos);
    }

    ID_TYPE elt = (*this)[freePos];
    pos[elt] = freePos;
    return elt;
  }

  // return the index of the first elt of a range of nb new elts
  unsigned int getFirstOfRange(unsigned int nb) {
    unsigned int freePos = this->size();
    unsigned int i = std::min(nbFree, nb);

    if (i) {
      setSize(freePos + i);
      nbFree -= i;
    }

    if (i < nb) {
      this->resize(freePos + nb);
      pos.resize(freePos + nb);

      for (; i < nb; ++i)
        (*this)[freePos + i] = ID_TYPE(freePos + i);
    }

    for (i = 0; i < nb; ++i)
      pos[(*this)[freePos + i]] = freePos + i;

    return freePos;
  }

  // push the elt in the free storage
  void free(ID_TYPE elt) {
    unsigned int curPos = pos[elt];
    unsigned int lastPos = this->size() - 1;

    ID_TYPE tmp;

    if (curPos != lastPos) {
      // swap the elt with the last one
      tmp = (*this)[lastPos];
      (*this)[lastPos] = (*this)[curPos];
      assert((*this)[curPos] == elt);
      (*this)[curPos] = tmp;
      pos[tmp] = curPos;
    }

    pos[elt] = UINT_MAX;

    if (lastPos) {
      // lastPos is now the beginning
      // of the freed elts
      ++nbFree;
      setSize(lastPos);
    } else {
      // all elts are freed so forget them
      nbFree = 0;
      setSize(0);
      pos.resize(0);
    }
  }

  // copy this into ids
  void copyTo(IdContainer<ID_TYPE> &ids) const {
    unsigned int sz = std::vector<ID_TYPE>::size() + nbFree;
    ids.reserve(sz);
    memcpy(ids.data(), this->data(), sz * sizeof(ID_TYPE));
    ids.pos.resize(sz);
    memcpy(ids.pos.data(), this->pos.data(), sz * sizeof(unsigned int));
    ids.nbFree = nbFree;
    ids.setSize(std::vector<ID_TYPE>::size());
  }

  // swap two elts
  void swap(ID_TYPE a, ID_TYPE b) {
    assert(isElement(a));
    assert(isElement(b));
    unsigned int pa = pos[a];
    unsigned int tmp = pos[b];
    pos[b] = pa;
    pos[a] = tmp;
    (*this)[pa] = b;
    (*this)[tmp] = a;
  }

  // recompute elts positions
  void reIndex() {
    unsigned int nbElts = this->size();
#ifdef _OPENMP
#pragma omp parallel for
#endif

    for (OMP_ITER_TYPE i = 0; i < nbElts; ++i)
      pos[(*this)[i]] = i;
  }

  // ascending sort
  void sort() {
    std::sort(this->begin(), this->end());
    reIndex();
  }
};

// used as nodes/edges container in GraphView
template <typename ID_TYPE>
class SGraphIdContainer : public std::vector<ID_TYPE> {
  // used to store the elts positions in the vector
  MutableContainer<unsigned int> pos;

public:
  SGraphIdContainer() {
    pos.setAll(UINT_MAX);
  }
  inline bool isElement(ID_TYPE elt) const {
    return (pos.get(elt) != UINT_MAX);
  }

  inline unsigned int getPos(ID_TYPE elt) const {
    assert(isElement(elt));
    return pos.get(elt);
  }

  void add(ID_TYPE elt) {
    assert(!isElement(elt));
    // put the elt at the end
    pos.set(elt, this->size());
    this->push_back(elt);
  }

  void clone(const std::vector<ID_TYPE> &elts) {
    static_cast<std::vector<ID_TYPE> &>(*this) = elts;
    unsigned int nb = elts.size();

    for (unsigned int i = 0; i < nb; ++i)
      pos.set(elts[i], i);
  }

  void remove(ID_TYPE elt) {
    assert(isElement(elt));
    // get the position of the elt to remove
    unsigned int i = pos.get(elt);
    assert(i < this->size());
    // put the last elt at the freed position
    unsigned int last = this->size() - 1;

    if (i < last)
      pos.set(((*this)[i] = (*this)[last]), i);

    // resize the container
    this->resize(last);
    // the elt no loger exist in the container
    pos.set(elt, UINT_MAX);
  }

  // ascending sort
  void sort() {
    std::sort(this->begin(), this->end());
    unsigned int nbElts = this->size();

    for (unsigned int i = 0; i < nbElts; ++i)
      pos.set((*this)[i], i);
  }
};
}

#endif

///@endcond
