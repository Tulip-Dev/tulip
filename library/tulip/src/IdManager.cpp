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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/IdManager.h"
#include <limits.h>
#include <assert.h>
using namespace std;

namespace tlp {

//-----------------------------------------------------------
bool IdManager::is_free(const unsigned int id) const {
  if (id < firstId) return true;
  if (id >= nextId) return true;
  if (freeIds.find(id)!=freeIds.end()) return true;
  return false;
}
//-----------------------------------------------------------
void IdManager::free(const unsigned int id) {
  if (id<firstId) return;
  if (id>= nextId) return;
  if (freeIds.find(id)!=freeIds.end()) return;

  if (firstId == nextId)
    return;
  if (id == firstId) {
    for(;;) {
      set<unsigned int>::iterator it = freeIds.find(++firstId);
      if (it == freeIds.end())
	break;
      freeIds.erase(it);
    }
  } else
    freeIds.insert(id);
}
//-----------------------------------------------------------
unsigned int IdManager::get() {
  return nextId++;
}
//-----------------------------------------------------------
void IdManager::getFreeId(unsigned int id) {
  assert(id > firstId);
  if (id >= nextId) {
    if (firstId == nextId)
      firstId = id;
    else {
      for (; nextId < id; ++nextId)
	freeIds.insert(nextId);
    }
    nextId = id + 1;
  } else {
    assert(freeIds.find(id) != freeIds.end());
    freeIds.erase(freeIds.find(id));
  }
}
//-----------------------------------------------------------
bool IdManager::hasFreeIds() const {
  return !freeIds.empty();
}
//-----------------------------------------------------------

}

//-----------------------------------------------------------
ostream& tlp::operator<<(ostream &os,const tlp::IdManager &idM) {
  os << endl << "--------------------------------------" << endl;
  os << "Id Manager Information :" << endl;
  os << "Minimum index :" << idM.firstId<< endl;
  os << "Maximum index :" << idM.nextId - 1 << endl;
  os << "Size          :" << idM.freeIds.size() << endl;
  os << "Fragmentation :" << (double)idM.freeIds.size() / (1+idM.nextId - idM.firstId) << endl;
  return os;
}
