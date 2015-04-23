/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

ParallelCoordsElementHighlighter::~ParallelCoordsElementHighlighter() {
  ParallelCoordinatesView *parallelView = dynamic_cast<ParallelCoordinatesView *>(view());

  if (parallelView)
    parallelView->resetHighlightedElements();
}

}
