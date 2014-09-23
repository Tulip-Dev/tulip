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
#include "rectanglePackingFonctions.h"
#include <cstdlib>

using namespace std;
using namespace tlp;

vector<Rectangle<float> > & RectanglePackingLimitRectangles(vector<Rectangle<float> > & v, const char * quality, PluginProgress * progress) {

  int numberOfPackedRectangles;
  vector<Rectangle<float> >::iterator itNewRect;
  int numberNewRect;

  RectanglePacking * rectPack = new RectanglePacking(v.size());

  /* we calculate the number of rectangles which are going to be placed in an optimal way*/
  numberOfPackedRectangles = rectPack->calculOfNumberOptimalRepositionnedRectangles(quality);

  /* we retrieve a pointer on the last rectangle which will be placed in an optimal way */
  itNewRect=v.begin();

  /* we go over all the rectangles to place in an optimal way */
  for(numberNewRect = 1; numberNewRect <= numberOfPackedRectangles; ++numberNewRect) {

    /* we calculate the co-ordinates of the new rectangle and those of the rectangles that the new rectangle eventually displaces, that is to say the rectangles placed to its right or above it */
    rectPack->optimalPositionOfNewRectangle(itNewRect);

    ++itNewRect;

    /* to follow the algorithm progression on through the PluginProgress*/
    if (progress != NULL)
      if (progress->progress(numberNewRect, numberOfPackedRectangles + 1) != TLP_CONTINUE) {
        return v;
      }
  }

  /* we definitively change the co-ordinates of the rectangles which have been placed in an optimal way */
  (rectPack->firstSequence)->allocateCoordinates();

  /* we calculate the co-ordinates of the rectangles which have not been packed in an optimal way */
  rectPack->defaultPositionRestOfRectangles(itNewRect, v.end());

  delete rectPack;

  /*added to enable the synchronisation of the PluginProgress closing and the end of the algorithm*/
  if (progress != NULL)
    if (progress->progress(numberNewRect, numberOfPackedRectangles + 1) != TLP_CONTINUE)
      return v;

  return v;
}

vector<Rectangle<float> > & RectanglePackingLimitPositions(vector<Rectangle<float> > & v, const char * quality, PluginProgress * progress) {

  /*useful variables for the PluginProgress*/
  int counter = 1;
  int entrySize = v.size();

  unsigned int numberTestedPositions;
  vector<Rectangle<float> >::iterator itNewRect;

  RectanglePacking * rectPack = new RectanglePacking(entrySize);

  /* we calculate the number of rectangles which will be placed in an optimal way */
  numberTestedPositions = rectPack->calculNumberOfTestedPositions(quality);

  /* we go over all the rectangles to pack in an optimal way */
  for(itNewRect=v.begin(); itNewRect!=v.end(); ++itNewRect) {

    /* we calculate the co-ordinates of the new rectangle and those of the rectangles that the new rectangle eventually displaces, that is to say the rectangles placed to its right or above it */
    rectPack->optimalPositionOfNewRectangleLimPos(itNewRect, numberTestedPositions);

    /* to follow the algorithm progression on through the PluginProgress*/
    if (progress != NULL)
      if (progress->progress(counter, entrySize + 1) != false)
        exit(1);

    ++counter;
  }

  /* we definitively change the co-ordinates of the rectangles which have been placed in an optimal way */
  (rectPack->firstSequence)->allocateCoordinates();

  delete rectPack;

  /*added to enable the synchronisation of the PluginProgress closing and the end of the algorithm*/
  if (progress != NULL)
    if (progress->progress(counter, entrySize + 1) != false)
      exit(1);

  return v;
}
