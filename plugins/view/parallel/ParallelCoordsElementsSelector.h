//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef PARALLELCOORDSELEMENTSSELECTOR_H_
#define PARALLELCOORDSELEMENTSSELECTOR_H_

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsElementsSelector : public ParallelCoordsGlEntitiesSelector {
  public :
    ParallelCoordsElementsSelector(Qt::MouseButton button = Qt::LeftButton,
                     Qt::KeyboardModifier modifier = Qt::NoModifier) : ParallelCoordsGlEntitiesSelector(button, modifier) {}
    ~ParallelCoordsElementsSelector(){}
    InteractorComponent *clone() { return new ParallelCoordsElementsSelector(mButton, kModifier); }
    void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView);
    void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection);
    void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection);
};

}

#endif /* PARALLELCOORDSELEMNTSSELECTOR_H_ */
