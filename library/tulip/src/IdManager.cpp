#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/IdManager.h"
#include <limits.h>
using namespace std;
using namespace tlp;
IdManager::IdManager():
  maxId(UINT_MAX),
  minId(UINT_MAX) {
}
//-----------------------------------------------------------
bool IdManager::is_free(const unsigned int id) const {
  if (id < minId) return true;
  if (id > maxId) return true;
  if (freeIds.find(id)!=freeIds.end()) return true;
  return false;
}
//-----------------------------------------------------------
void IdManager::free(const unsigned int id) {
  if (id<minId) return;
  if (id>maxId) return;
  if (freeIds.find(id)!=freeIds.end()) return;

  if (maxId == minId) {
    maxId = UINT_MAX;
    minId = UINT_MAX;
    return;
  }
  //remove all free id
  if (id == maxId) {
    set<unsigned int>::reverse_iterator it;
    it = freeIds.rbegin();
    unsigned int lastId = maxId;
    bool eraseOk = false;
    while (it!=freeIds.rend() && (lastId-(*it))==1){
      --lastId;
      ++it;
      eraseOk=true;
    }
    if (eraseOk) {
      maxId = lastId-1;
      if (it==freeIds.rend()) freeIds.clear();
      else {
	set<unsigned int>::const_iterator it1,it2;
	it1 = freeIds.end();
	it2 = freeIds.find(lastId);
	freeIds.erase(it2,it1);
      }
    }
    else
      --maxId;
  }
  else
    if (id == minId) {
      set<unsigned int>::const_iterator it;
      it = freeIds.begin();
      unsigned int lastId = minId;
      bool eraseOk = false;
      while (it!=freeIds.end() && ((*it)-lastId) == 1 ) {
	++lastId; 
	++it;
	eraseOk = true;
      }
      if (eraseOk) {
	minId = lastId + 1;
	if (it==freeIds.end()) freeIds.clear();
	else {
	  set<unsigned int>::const_iterator it1,it2;
	  it1=freeIds.begin();
	  it2=freeIds.find(minId-1);
	  freeIds.erase(it1,++it2);
	}
      }
      else
	++minId;
    }
    else
      freeIds.insert(id);
}
//-----------------------------------------------------------
unsigned int IdManager::get() {
  unsigned int newId;
  if (freeIds.empty()) {
    if (minId == UINT_MAX) {
      newId = 0;
      minId = 0;
      maxId = 0;
    } else 
      if (minId > 0) 
	newId = --minId;
      else
	newId = ++maxId;
  }
  else {
    set<unsigned int>::iterator it=freeIds.begin();
    newId=*it;
    freeIds.erase(it);
  }
  return newId;
}
//-----------------------------------------------------------
Iterator<unsigned int>* IdManager::getUsedId() const{
  return new IdManagerIterator(*this);
}
//-----------------------------------------------------------
ostream& tlp::operator<<(ostream &os,const tlp::IdManager &idM) {
  os << endl << "--------------------------------------" << endl;
  os << "Id Manager Information :" << endl;
  os << "Minimum index :" << idM.minId<< endl;
  os << "Maximum index :" << idM.maxId<< endl;
  os << "Size          :" << idM.freeIds.size() << endl;
  os << "Fragmentation :" << (double)idM.freeIds.size() / (1+idM.maxId-idM.minId) << endl;
  /*
  os << "Set of free index  :" << endl;
  os << "[";
  set<unsigned int>::const_iterator it;
  for (it=idM.freeIds.begin();it!=idM.freeIds.end();++it)
    os << *it << " ";
  os << "]" << endl;
  os << "Set of used index  :" << endl;
  os << "[";
  Iterator<unsigned int> *itD=idM.getUsedId();
  for (;itD->hasNext();)
    os << itD->next() << " ";
  delete itD;
  os << "]" << endl;
  os << "--------------------------------------" << endl;
  */
  return os;
}
//-----------------------------------------------------------
IdManagerIterator::IdManagerIterator(const IdManager &idMan):
    index(idMan.minId),it(idMan.freeIds.begin()),idMan(idMan)
{}
//-----------------------------------------------------------
IdManagerIterator::~IdManagerIterator(){};
//-----------------------------------------------------------
unsigned  int IdManagerIterator:: next() {
  unsigned int tmp(index);
  ++index;
  while( it != idMan.freeIds.end()) {
    if (index<*it) return tmp;
    ++index;
    ++it;
  }
  return tmp;
}
//-----------------------------------------------------------
bool IdManagerIterator::hasNext() {
  return (index!=UINT_MAX && index<=idMan.maxId);
}
//-----------------------------------------------------------
