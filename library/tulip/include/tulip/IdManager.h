//-*-c++-*-
/*
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef DOXYGEN_NOTFOR_DEVEL

#ifndef _TULIPIDMANAGER_H
#define _TULIPIDMANAGER_H
#include <set>
#include <iostream>
#include "tulip/Iterator.h"

namespace tlp {

class IdManagerIterator;

/// class for the management of the identifiers : node, edge
class IdManager {

public:
  IdManager();
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
   * the memory space of the free list, and the fragmetation
   * of the ids.
   */
  unsigned int get();
  /**
   * Returns an iterator on all the used id. Warning, if
   * the idManager is modified (free, get) this iterator 
   * will be invalidate.
   */
  Iterator<unsigned int>* getUsedId() const;

  friend std::ostream& operator<<(std::ostream &,const IdManager &);
  friend class IdManagerIterator;
  friend class IdManagerTest;

  private:
  std::set<unsigned int> freeIds;  
  unsigned int maxId;
  unsigned int minId;
};
//======================================================
/// Iterator for id managers
#ifndef DOXYGEN_NOTFOR_DEVEL
class IdManagerIterator:public Iterator<unsigned int>  {
public:
  IdManagerIterator(const IdManager &idMan);
  ~IdManagerIterator();
  unsigned int next();
  bool hasNext();
private:
  unsigned int index;
  std::set<unsigned int>::const_iterator it;
  const IdManager &idMan;
};
#endif
//======================================================
std::ostream& operator<<(std::ostream &,const IdManager &);

}

#endif
#endif //DOXYGEN_NOTFOR_DEVEL
