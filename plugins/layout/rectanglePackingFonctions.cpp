/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include "rectanglePackingFonctions.h"
#include <cstdlib>

using namespace std;
using namespace tlp;

bool RectanglePackingLimitRectangles(vector<Rectangle<float>> &v, const char *quality,
                                     PluginProgress *progress) {

  int numberOfPackedRectangles;
  vector<Rectangle<float>>::iterator itNewRect;
  int numberNewRect;

  RectanglePacking rectPack(v.size());

  /* we calculate the number of rectangles which are going to be placed in an optimal way*/
  numberOfPackedRectangles = rectPack.calculOfNumberOptimalRepositionnedRectangles(quality);

  /* we retrieve a pointer on the last rectangle which will be placed in an optimal way */
  itNewRect = v.begin();

  /* we go over all the rectangles to place in an optimal way */
  for (numberNewRect = 1; numberNewRect <= numberOfPackedRectangles; ++numberNewRect) {

    /* we calculate the coordinates of the new rectangle and those of the rectangles that the new
     * rectangle eventually displaces, that is to say the rectangles placed to its right or above it
     */
    rectPack.optimalPositionOfNewRectangle(itNewRect);

    ++itNewRect;

    /* to follow the algorithm progression on through the PluginProgress*/
    if (progress &&
        (progress->progress(numberNewRect, numberOfPackedRectangles + 1) != TLP_CONTINUE)) {
      return false;
    }
  }

  /* we definitively change the coordinates of the rectangles which have been placed in an optimal
   * way */
  (rectPack.firstSequence)->allocateCoordinates();

  /* we calculate the coordinates of the rectangles which have not been packed in an optimal way */
  rectPack.defaultPositionRestOfRectangles(itNewRect, v.end());

  /* added to enable the synchronisation of the PluginProgress closing and the end of the
   * algorithm*/
  return progress ? (progress->progress(numberNewRect, numberOfPackedRectangles + 1) != TLP_CANCEL)
                  : true;
}

bool RectanglePackingLimitPositions(vector<Rectangle<float>> &v, const char *quality,
                                    PluginProgress *progress) {

  /*useful variables for the PluginProgress*/
  int counter = 1;
  int entrySize = v.size();

  unsigned int numberTestedPositions;
  vector<Rectangle<float>>::iterator itNewRect;

  RectanglePacking rectPack(entrySize);

  /* we calculate the number of rectangles which will be placed in an optimal way */
  numberTestedPositions = rectPack.calculNumberOfTestedPositions(quality);

  /* we go over all the rectangles to pack in an optimal way */
  for (itNewRect = v.begin(); itNewRect != v.end(); ++itNewRect) {

    /* we calculate the coordinates of the new rectangle and those of the rectangles that the new
     * rectangle eventually displaces, that is to say the rectangles placed to its right or above it
     */
    rectPack.optimalPositionOfNewRectangleLimPos(itNewRect, numberTestedPositions);

    /* to follow the algorithm progression on through the PluginProgress*/
    if (progress && (progress->progress(counter, entrySize + 1) != TLP_CONTINUE))
      return false;

    ++counter;
  }

  /* we definitively change the coordinates of the rectangles which have been placed in an optimal
   * way */
  (rectPack.firstSequence)->allocateCoordinates();

  /* added to enable the synchronisation of the PluginProgress closing and the end of the
   * algorithm*/
  return progress ? (progress->progress(counter, entrySize + 1) != TLP_CANCEL) : true;
}
