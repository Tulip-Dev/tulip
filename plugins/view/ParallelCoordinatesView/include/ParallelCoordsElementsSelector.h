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

#ifndef PARALLELCOORDSELEMENTSSELECTOR_H_
#define PARALLELCOORDSELEMENTSSELECTOR_H_

#include "ParallelCoordsGlEntitiesSelector.h"

namespace tlp {

class ParallelCoordsElementsSelector : public ParallelCoordsGlEntitiesSelector {
public:
  ParallelCoordsElementsSelector(Qt::MouseButton button = Qt::LeftButton,
                                 Qt::KeyboardModifier modifier = Qt::NoModifier)
      : ParallelCoordsGlEntitiesSelector(button, modifier) {}
  ~ParallelCoordsElementsSelector() override {}
  void unselectAllEntitiesHandler(ParallelCoordinatesView *parallelView) override;
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y,
                               bool addToSelection) override;
  void selectedEntitiesHandler(ParallelCoordinatesView *parallelView, int x, int y, int w, int h,
                               bool addToSelection) override;
};
} // namespace tlp

#endif /* PARALLELCOORDSELEMNTSSELECTOR_H_ */
