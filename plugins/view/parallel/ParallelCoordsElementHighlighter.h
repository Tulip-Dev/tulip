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

#ifndef PARALLELCOORDSELEMENTHIGHLIGHTER_H_
#define PARALLELCOORDSELEMENTHIGHLIGHTER_H_

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsElementHighlighter : public ParallelCoordsGlEntitiesSelector {
  public :
    ParallelCoordsElementHighlighter(Qt::MouseButton button = Qt::LeftButton,
                     Qt::KeyboardModifier modifier = Qt::NoModifier) : ParallelCoordsGlEntitiesSelector(button, modifier) {}
    ~ParallelCoordsElementHighlighter(){}
    InteractorComponent *clone() { return new ParallelCoordsElementHighlighter(mButton, kModifier); }
    void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView);
    void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection);
    void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection);
};

}

#endif /* PARALLELCOORDSELEMNTHIGHLIGHTER_H_ */
