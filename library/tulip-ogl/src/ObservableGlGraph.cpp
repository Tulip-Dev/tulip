#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableGlGraph.h"

using namespace std;
using namespace tlp;

void ObservableGlGraph::notifyMoveCamera(GlGraph *glGraph) {
  set<GlGraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GlGraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->moveCamera(glGraph);
}

void ObservableGlGraph::notifyMoveObject(GlGraph *glGraph) {
  set<GlGraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GlGraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->moveObject(glGraph);
}

void ObservableGlGraph::notifyDraw(GlGraph *glGraph) {
  set<GlGraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GlGraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->draw(glGraph);
}

void ObservableGlGraph::notifyDestroy(GlGraph *glGraph) {
  set<GlGraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GlGraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->destroy(glGraph);
}
