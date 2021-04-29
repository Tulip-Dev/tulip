/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "ParallelCoordsElementsSelector.h"
#include "ParallelCoordinatesView.h"

using namespace std;

namespace tlp {

void ParallelCoordsElementsSelector::unselectAllEntitiesHandler(
    ParallelCoordinatesView *parallelView) {
  parallelView->resetSelection();
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView,
                                                             int x, int y, bool addToSelection) {
  parallelView->setDataUnderPointerSelectFlag(x, y, addToSelection);
}

void ParallelCoordsElementsSelector::selectedEntitiesHandler(ParallelCoordinatesView *parallelView,
                                                             int x, int y, int w, int h,
                                                             bool addToSelection) {
  parallelView->setDataInRegionSelectFlag(x, y, w, h, addToSelection);
}
} // namespace tlp
