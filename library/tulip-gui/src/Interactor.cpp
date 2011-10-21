#include "tulip/Interactor.h"

#include <tulip/ForEach.h>
#include <tulip/View.h>

using namespace std;
using namespace tlp;

QMap<std::string,QList<std::string> > InteractorLister::_compatibilityMap = QMap<std::string,QList<std::string> >();

bool interactorLessThan(Interactor* a, Interactor* b) {
  return a->priority() > b->priority();
}

void InteractorLister::initInteractorsDependencies() {
  QMap<Interactor*,string> interactorName;

  string it;
  forEach(it,availablePlugins())
  interactorName[getPluginObject(it,NULL)] = it;

  string viewName;
  forEach(viewName, ViewLister::availablePlugins()) {
    QList<Interactor*> sortedList;
    foreach(Interactor* i,interactorName.keys()) {
      if (i->isCompatible(viewName))
        sortedList << i;
    }
    qSort(sortedList.begin(),sortedList.end(),interactorLessThan);
    QList<string> compatibleNames;
    foreach(Interactor* i,sortedList)
    compatibleNames << interactorName[i];
    _compatibilityMap[viewName] = compatibleNames;
  }

  foreach(Interactor* i, interactorName.keys())
  delete i;
}

QList<string> InteractorLister::compatibleInteractors(const std::string &viewName) {
  return _compatibilityMap[viewName];
}
