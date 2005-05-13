//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
 Last modification : 20/08/2001
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _TULIPIDMANAGER_H
#define _TULIPIDMANAGER_H
#include <set>
#include <iostream>
#include "Iterator.h"

class IdManagerIterator;

class IdManager {
public:
  IdManager();
  bool is_free(unsigned int) const;
  /**
     Free the id given in parameter. This id
     will be accessible by a the get function.
  */
  void free(const unsigned int);
  /**
     Return a new id, the returned id is choosen to minimize
     the memory space of the free list, and the fragmetation
     of the ids.
  */
  unsigned int get();
  Iterator<unsigned int>* getUsedId() const;
  
  friend std::ostream& operator<<(std::ostream &,const IdManager &);
  friend class IdManagerIterator;

private:
  std::set<unsigned int> freeIds;  
  unsigned int maxId;
  unsigned int minId;
};
//======================================================
class IdManagerIterator:public Iterator<unsigned int>  {
public:
  IdManagerIterator(const IdManager &idMan);
  ~IdManagerIterator();
  unsigned  int  next();
  bool hasNext();
private:
  unsigned int index;
  std::set<unsigned int>::const_iterator it;
  const IdManager &idMan;
};

std::ostream& operator<<(std::ostream &,const IdManager &);

#endif
