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
#ifndef RECTANGLEPACKING_H
#define RECTANGLEPACKING_H

#include <cmath>
#include <vector>
#include "Number.h"
#include <tulip/PluginProgress.h>
#include "RectangleRelativePositionList.h"

/**
 *This class is used for the implementation of the greedy algorithm.
 */
class RectanglePacking {
public:

  RectangleRelativePositionList * firstSequence;/**< List which is useful for the stocking of the first sequence of the even sequence. */

  int *placesOfRectanglesInSecondSequence; /**< Array which is used for the stocking of the positions of the rectangles in the second sequence. */

  int numberOfPositionnedRectangles; /**< Current value of the number of packed rectangles. */
  int numberOfRectangles; /**< Value of the number of rectangles to pack. */

  int bestPlaceInFirstSequence; /**< Current best position of a new rectangle to pack, in the first sequence, since the beginning of the position tests. */
  int bestPlaceInSecondSequence; /**< Current best position of a new rectangle to pack, in the second sequence, since the beginning of the position tests. */

  float newRectangleWidth; /**< Width of a new rectangle to pack. */
  float newRectangleHeight; /**< Height of a new rectangle to pack. */

  float newRectangleLeftAbscissa; /**< Left abscissa of a new rectangle to pack, for a tested position. */
  float newRectangleLowOrdinate; /**< Low ordinate of a new rectangle to pack, for a tested position. */

  float bestRectangleLeftAbscissa; /**< Best left abscissa of a new rectangle to pack since the beginning of the position tests. */
  float bestRectangleLowOrdinate; /**< Best low ordinate of a new rectangle to pack since the beginning of the position tests. */

  float maxWidthOfBoundingBox; /**< Width of the rectangle including all the packed rectangles and a new rectangle to pack, for a tested position. */
  float maxHeightOfBoundingBox; /**< Width of the rectangle including all the packed rectangles and a new rectangle to pack, for a tested position. */

  float bestWidthOfBoundingBox; /**< Best width of the rectangle including all the packed rectangles and a new rectangle to pack since the beginning of the position tests. */
  float bestHeightOfBoundingBox; /**< Best height of the rectangle including all the packed rectangles and a new rectangle to pack since the beginning of the position tests. */

  /**
   *constructor of the RectanglePacking class.
   *@param numberRects number of rectangles to pack.
   */
  RectanglePacking(int numberRects);

  /**
   *destroyer of the RectanglePacking class.
   */
  ~RectanglePacking();

  /**
   *Place the rectangles not packed in an optimal way around the rectangle
   *including the rectangles packed in an optimal way.
   */
  void defaultPositionRestOfRectangles(std::vector<tlp::Rectangle<float> >::iterator itlim, std::vector<tlp::Rectangle<float> >::iterator itend);

  /**
   *Search the best co-ordinates of a rectangle in order to pack it in the
   *best way. Test the results obtained with all the possible positions in
   * the even sequence.
   */
  void optimalPositionOfNewRectangle(std::vector<tlp::Rectangle<float> >::iterator itNewRect);

  /**
   *Search the co-ordinates of a rectangle for the positions tested in the
   *first and in the second sequence of the even sequence. Search the
   *dimensions of the rectangle including all the packed rectangles.
   *@param positionInFirstSequence position tested in the first sequence.
   *@param positionInSecondSequence position tested in the second sequence.
   */
  std::list<RectangleRelativePosition>::iterator testOfPositionOfNewRectangle(int positionInFirstSequence, int positionInSecondSequence);

  /**
   *Search the co-ordinates of a rectangle for the positions tested in the
   *first and in the second sequence of the even sequence. Search the
   *dimensions of the rectangle including the rectangles placed on the left
   *and below the new rectangle.
   *@param positionInFirstSequence position tested in the first sequence.
   *@param positionInSecondSequence position tested in the second sequence.
   */
  std::list<RectangleRelativePosition>::iterator positionOfNewRectangle(int positionInFirstSequence, int positionInSecondSequence);

  /**
   *Determine if the rectangle pointed by itRectLeftOrBelowOfNewRect has a
   *right abscissa or a high ordonate higher than those of rectangles yet
   *visited.
   *@param itRectLeftOrBelowOfNewRect pointer on one of the structures of
   *RectangleRelativePosition corresponding to a rectangle places on the left
   *or below the tested position of the new rectangle.
   *@param positionInFirstSequence position tested in the first sequence.
   *@param positionInSecondSequence position tested in the second sequence.
   */
  void coordinatesOfNewRectangle(std::list<RectangleRelativePosition>::iterator itRectLeftOrBelowOfNewRect, int positionInFirstSequence, int positionInSecondSequence);

  /**
   *Stock in the fields maxWidthOfBoundingBox and maxHeightOfBoundingBox
   *the dimensions of the rectangle including the rectangles on the left
   *and below the considered rectangle.
   */
  void dimensionsBoundingBoxOfRectanglesLeftOrBelowNewRectangle(std::list<RectangleRelativePosition>::iterator itRectLeftOrBelowOfNewRect);

  /**
   *Search the dimensions of the rectangle including the new rectangle and
   *the rectangles on the left and below it.
   */
  void dimensionsBoundingBoxOfNewRectangleAndRectanglesLeftOrBelow();

  /**
   *Calculate again the co-ordinates of the rectangles places on the right and
   *above the new rectangle. Calculate the dimensions of the rectangle
   *including all the packed rectangles.
   *@param positionInFirstSequence position tested in the first sequence.
   *@param positionInSecondSequence position tested in the second sequence.
   */
  void repositionOfRectanglesRightOrAboveNewRectangle(std::list<RectangleRelativePosition>::iterator itFirstRectangleRightOrAboveOfNewRectangle, int positionInFirstSequence, int positionInSecondSequence);

  /**
   *Compare the right abscissa and the high ordonate of the new rectangle,
   * with the left abscissa and the low ordonate of the rectangle pointed by
   *itRectOfSequenceToReposition.
   *@param itRectOfSequenceToReposition pointer on one rectangle placed on the
   *right or above the new rectangle.
   *@param positionInSecondSequence position tested in the second sequence.
   */
  void modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionNewRectangle(std::list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition, int positionInSecondSequence);

  /**
    *Compare the left abscissa and the low ordonate of the rectangle pointed by
    *itRectOfSequenceToReposition with the right abscissa  and the high
    *ordonate of all the rectangles placed in the first sequence between
    *the first rectangle on the right of the tested position and the rectangle
    *pointed by itRectOfSequenceToReposition.
    *@param itFirstRectangleRightOrAboveOfNewRectangle pointer on the first
    *rectangle on the right or above the new rectangle.
    *@param itRectOfSequenceToReposition pointer on the rectangles places on the
    *right or above the new rectangle.
    */
  void modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionAlreadyRepositionnedRectangles(std::list<RectangleRelativePosition>::iterator itFirstRectangleRightOrAboveOfNewRectangle, std::list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition);

  /**
   *Search the dimensions of the rectangle including the rectangles on the
   *left and below the considered rectangle.
   */
  void dimensionsBoundingBoxOfAllOptimalPositionnedRectangles(std::list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition);

  /**
   * Modify the even sequence when all the position tests, for a new rectangle
   * to pack, have been made.
   */
  void modificationOfSequencePair(std::vector<tlp::Rectangle<float> >::iterator itNewRect, std::list<RectangleRelativePosition>::iterator itBestPositionInFirstSequence);

  /**
   *Calculate the number of rectangles we can pack in an optimal way without
   *going beyond the complexity desired.
   *@param quality give the complexity desired.
   *@return an integer equal to the number of rectangles we want to pack
   * in an optimal way.
   */
  int calculOfNumberOptimalRepositionnedRectangles(const char * quality);

  /**
   *Calculate the ratio width/height or height/width so that the return >=1.
   */
  float calculateRatio();

  /**
   *decide if a new rectangle is going to be placed in line or in column.
   */
  void lineOrColumnToStart(bool & boolWidth, bool & boolHeight);

  /**
   *End a line and decide if the rectangle pointed is going to start a line
   *or a column.
   */
  void endOfLine(float & heightTemp, float & widthTemp, bool & boolWidth, bool & boolHeight);

  /**
   *End a column and decide if the rectangle pointed is going to start a line
   *or a column.
   */
  void endOfColumn(float & widthTemp, float & heightTemp, bool & boolWidth, bool & boolHeight);

  /**
   *We start or continue a new line if the width of the new rectangle to pack,
   * added to the width of the current line, is strictly lower to the width of
   * the including rectangle.
   */
  void continueLine(std::vector<tlp::Rectangle<float> >::iterator itr, float & widthTemp,float & heightTemp, bool & boolWidth, bool & boolHeight);

  /**
  *We start or continue a new column if the height of the new rectangle to
  *pack, added to the height of the current column, is strictly lower to the
  *height of the including rectangle.
  */
  void continueColumn(std::vector<tlp::Rectangle<float> >::iterator itr, float & widthTemp,float & heightTemp, bool & boolWidth, bool & boolHeight);

  void optimalPositionOfNewRectangleLimPos(std::vector<tlp::Rectangle<float> >::iterator itNewRect, int numberTestedPositions);

  /**
   *Calculate the number of rectangles we can pack without go beyond the
   *complexity desired.
   */
  int calculNumberOfTestedPositions(const char * quality);
};

#endif
