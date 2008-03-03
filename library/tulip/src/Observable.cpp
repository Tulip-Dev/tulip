#include <cassert>
#include <iostream>
#include "tulip/Observable.h"


using namespace std;
using namespace tlp;

#ifdef _WIN32 
#ifdef DLL_EXPORT
int Observable::holdCounter=0;
ObserverMap Observable::holdMap;
#endif
#else
int Observable::holdCounter=0;
ObserverMap Observable::holdMap;
#endif

static bool unholdLock=false;

//===============================================================
void Observable::notifyDestroy() {
  //  cerr << "Observable::notifyObservers" << endl;
  std::list<Observer*> tmpList(observersList);
  list<Observer*>::iterator itlObs;
  for (itlObs=tmpList.begin(); itlObs!=tmpList.end(); ++itlObs)
    (*itlObs)->observableDestroyed(this);
}
//===============================================================
void Observable::notifyObservers() {
  if (unholdLock) {
    cerr << "Cannot notifyObservers during unholdings" << endl;
    return;
  }
  //  cerr << "Observable::notifyObservers" << endl;
  list<Observer*>::iterator itlObs;
  assert(holdCounter>=0);
  if (holdCounter)
    for (itlObs=observersList.begin();itlObs!=observersList.end();++itlObs)
      holdMap[*itlObs].insert(this);
  else {
    set<Observable *> tmpSet;
    std::list<Observer*> tmpList(observersList);
    tmpSet.insert(this);
    for (itlObs=tmpList.begin();itlObs!=tmpList.end();++itlObs)
      (*itlObs)->update(tmpSet.begin(),tmpSet.end());
  }
}
//===============================================================
void Observable::holdObservers() {
  //  cerr << __PRETTY_FUNCTION__ << " :=> " << holdCounter << endl;
  assert(holdCounter>=0);
  holdCounter++;
}
//===============================================================
void Observable::unholdObservers() {
  assert(holdCounter>=0);
  //  cerr << __PRETTY_FUNCTION__ << " :=> " << holdCounter << endl;
  holdCounter--;
  if (unholdLock) return;
  unholdLock=true; 
  if (holdCounter==0) {
    ObserverMap tmp(holdMap);
    holdMap.clear();
    ObserverMap::iterator itMObs;
    for (itMObs=tmp.begin();itMObs!=tmp.end();++itMObs) {
      itMObs->first->update(itMObs->second.begin(),itMObs->second.end());
    }
  }
  unholdLock=false;
}
//===============================================================
