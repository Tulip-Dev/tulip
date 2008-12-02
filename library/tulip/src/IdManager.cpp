#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/IdManager.h"
#include <limits.h>
using namespace std;
using namespace tlp;
IdManager::IdManager():
  nextId(0),
  firstId(0) {
}
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
Iterator<unsigned int>* IdManager::getIds() const{
  return new IdManagerIterator(*this);
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
//-----------------------------------------------------------
IdManagerIterator::IdManagerIterator(const IdManager &idMan):
  current(idMan.firstId), it(idMan.freeIds.begin()), last(idMan.nextId), idManager(idMan)
{
  set<unsigned int>::reverse_iterator it;
  it = idMan.freeIds.rbegin();
  while(it != idMan.freeIds.rend() && (*it) == last - 1) {
    --last;
    ++it;
  }
}
//-----------------------------------------------------------
IdManagerIterator::~IdManagerIterator(){};
//-----------------------------------------------------------
unsigned  int IdManagerIterator:: next() {
  unsigned int tmp = current;
  ++current;
  while( it != idManager.freeIds.end()) {
    if (current < *it) return tmp;
    ++current;
    ++it;
  }
  return tmp;
}
//-----------------------------------------------------------
bool IdManagerIterator::hasNext() {
  return (current < last);
}
//-----------------------------------------------------------
