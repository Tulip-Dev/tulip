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

#include "ParallelCoordsElementsSelector.h"

using namespace std;

namespace tlp {

void ParallelCoordsElementsSelector::unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) {
	parallelView->resetSelection();
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) {
	parallelView->setDataUnderPointerSelectFlag(x, y, addToSelection);
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) {
	parallelView->setDataInRegionSelectFlag(x, y, w, h, addToSelection);
}

}
