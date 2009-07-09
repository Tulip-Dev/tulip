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

#include "ParallelCoordsElementHighlighter.h"

using namespace std;

namespace tlp {

void ParallelCoordsElementHighlighter::unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) {
	parallelView->resetHighlightedElements();
}

void ParallelCoordsElementHighlighter::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection) {
	parallelView->highlightDataUnderPointer(x, y, addToSelection);
}

void ParallelCoordsElementHighlighter::selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection) {
	parallelView->highlightDataInRegion(x, y, w, h, addToSelection);
}

}
