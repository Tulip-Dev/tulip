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
#ifndef RECTANGLERELATIVEPOSITIONLIST_H
#define RECTANGLERELATIVEPOSITIONLIST_H

#include <vector>
#include <list>
#include <tulip/Rectangle.h>

/**
 *This structure enables the stocking of the datas corresponding to the search
 *of the optimal position of the rectangle designated by the field
 *rectangleIterator.
 */
struct RectangleRelativePosition {
  std::vector<tlp::Rectangle<float> >::iterator rectangleIterator;/**< pointer towards the rectangle whose position optimal is searched.*/
  int rectangleNumber; /**< the number which identifies the rectangle. */
  float rectangleWidth; /**< the width of the rectangle. */
  float rectangleHeight;/**< the height of the rectangle. */
  float rectangleLeftAbscissa;/**< the left abscissa of the rectangle chosen after the position tests. */
  float rectangleLowOrdinate;/**< the low ordinate of the rectangle chosen after the position tests. */
  float rectangleTemporaryLeftAbscissa;/**< the left abscissa of the rectangle for the current test. */
  float rectangleTemporaryLowOrdinate;/**< the low ordinate of the rectangle for the current test. */
  float rectangleTemporaryBestLeftAbscissa;/** the best left abscissa of the rectangle calculated since the beginning of the tests. */
  float rectangleTemporaryBestLowOrdinate;/** the best low ordinate of the rectangle calculated since the beginning of the tests. */
};

/**
 *This class is a list of structures of RectangleRelativePosition. It enables
 *the stocking of the first sequence of the even sequence made up as we insert
 *the new rectangles.
 */
class RectangleRelativePositionList:public std::list<RectangleRelativePosition> {

public:
  /**
   *Add a structure RectangleRelativePosition to the list.
   */
  void addRectangleRelativePosition(std::vector<tlp::Rectangle<float> >::iterator itr,
                                    int numRect, float wdth, float hght,
                                    float x, float y,
                                    std::list<RectangleRelativePosition>::iterator itRectangleRelativePosition);

  /**
   *Modify the co-ordinates of the Rectangles pointed by the fields
   *RectangleIterator of the elements of the list. These new co-ordinates are
   *calculated from the values of the fields rectangleLeftAbscissa and
   *rectangleLowOrdinate.
   */
  void allocateCoordinates();

  /**
   *Stock the values of the fields rectangleTemporaryLeftAbscissa and
   *rectangleTemporaryLowOrdinate in the fields rectangle
   *TemporaryBestLeftAbscissa and rectangleTemporaryBestLowOrdinate for all
   *the elements of the list from one position in the list.
   */
  void stockOfTemporaryBestCoordinates(int bestPlaceInFirstSequence);

};

#endif
