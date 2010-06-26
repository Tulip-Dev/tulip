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
