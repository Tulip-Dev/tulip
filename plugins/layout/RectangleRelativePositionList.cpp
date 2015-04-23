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
#include "RectangleRelativePositionList.h"

using namespace std;
using namespace tlp;

void RectangleRelativePositionList::addRectangleRelativePosition(vector<Rectangle<float> >::iterator itr, int numRect, float wdth, float hght, float x, float y, list<RectangleRelativePosition>::iterator itRectangleRelativePosition) {

  RectangleRelativePosition newRectangleRelativePosition;
  newRectangleRelativePosition.rectangleIterator = itr;
  newRectangleRelativePosition.rectangleNumber = numRect;
  newRectangleRelativePosition.rectangleWidth = wdth;
  newRectangleRelativePosition.rectangleHeight = hght;
  newRectangleRelativePosition.rectangleLeftAbscissa = x;
  newRectangleRelativePosition.rectangleLowOrdinate = y;

  this->insert(itRectangleRelativePosition, newRectangleRelativePosition);
}

void RectangleRelativePositionList::allocateCoordinates() {

  for(list<RectangleRelativePosition>::iterator itr=this->begin(); itr!=this->end(); ++itr) {
    (*(itr->rectangleIterator))[0][0] = itr->rectangleLeftAbscissa;
    (*(itr->rectangleIterator))[0][1] = itr->rectangleLowOrdinate;
    (*(itr->rectangleIterator))[1][0] = (*(itr->rectangleIterator))[0][0] + itr->rectangleWidth;
    (*(itr->rectangleIterator))[1][1] = (*(itr->rectangleIterator))[0][1]  + itr->rectangleHeight;
  }
}

void RectangleRelativePositionList::stockOfTemporaryBestCoordinates(int bestPlaceInFirstSequence) {

  list<RectangleRelativePosition>::reverse_iterator itRectToReposition = this->rbegin();
  int positionRectToReposition;

  for(positionRectToReposition = this->size(); positionRectToReposition>=bestPlaceInFirstSequence && itRectToReposition != this->rend(); --positionRectToReposition) {
    itRectToReposition->rectangleTemporaryBestLeftAbscissa = itRectToReposition->rectangleTemporaryLeftAbscissa;
    itRectToReposition->rectangleTemporaryBestLowOrdinate = itRectToReposition->rectangleTemporaryLowOrdinate;
    ++itRectToReposition;
  }
}
