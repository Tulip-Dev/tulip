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
#ifndef INTERACTORPRIORITY_H_
#define INTERACTORPRIORITY_H_

namespace tlp {

// List of standard interactors priorities
// just insert a new one at the right place
class StandardInteractorPriority {
public:
  enum priority {
    None = 0,
    ViewInteractor1,
    ViewInteractor2,
    ViewInteractor3,
    ViewInteractor4,
    ViewInteractor5,
    ViewInteractor6,
    ViewInteractor7,
    MagnifyingGlass,
    FishEye,
    NeighborhoodHighlighter,
    ZoomOnRectangle,
    DeleteElement,
    EditEdgeBends,
    AddNodesOrEdges,
    PathSelection,
    FreeHandSelection,
    RectangleSelectionModifier,
    RectangleSelection,
    GetInformation,
    Navigation
  };
};

}

#endif
