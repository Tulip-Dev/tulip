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

#ifndef PARALLELCOORDSELEMENTHIGHLIGHTER_H_
#define PARALLELCOORDSELEMENTHIGHLIGHTER_H_

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsElementHighlighter : public ParallelCoordsGlEntitiesSelector {
public :
  ParallelCoordsElementHighlighter(Qt::MouseButton button = Qt::LeftButton,
                                   Qt::KeyboardModifier modifier = Qt::NoModifier) : ParallelCoordsGlEntitiesSelector(button, modifier) {}
  ~ParallelCoordsElementHighlighter();
  void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView);
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, bool addToSelection);
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h, bool addToSelection);
};

}

#endif /* PARALLELCOORDSELEMNTHIGHLIGHTER_H_ */
