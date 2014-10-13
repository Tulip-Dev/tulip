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
#include "RectanglePacking.h"
#include <cstring>
#include <float.h> //FLT_MAX
#include <climits> //INT_MAX

using namespace std;
using namespace tlp;

RectanglePacking::RectanglePacking(int numberRects) : firstSequence(new RectangleRelativePositionList()), placesOfRectanglesInSecondSequence(new int[numberRects]),
  numberOfPositionnedRectangles(0), numberOfRectangles(numberRects), bestPlaceInFirstSequence(INT_MAX), bestPlaceInSecondSequence(INT_MAX),
  newRectangleWidth(FLT_MAX), newRectangleHeight(FLT_MAX), newRectangleLeftAbscissa(FLT_MAX),
  newRectangleLowOrdinate(FLT_MAX), bestRectangleLeftAbscissa(FLT_MAX), bestRectangleLowOrdinate(FLT_MAX), maxWidthOfBoundingBox(FLT_MAX),
  maxHeightOfBoundingBox(FLT_MAX), bestWidthOfBoundingBox(FLT_MAX), bestHeightOfBoundingBox(FLT_MAX) {
}

RectanglePacking::~RectanglePacking() {
  delete firstSequence;
  delete [] placesOfRectanglesInSecondSequence;
}

void RectanglePacking::optimalPositionOfNewRectangle(vector<Rectangle<float> >::iterator itNewRect) {
  float ratio;
  Number bestRatio(Number::infini);
  float perimeterOfBoundingRectangle;
  Number bestPerimeterOfBoundingRectangle(Number::infini);
  int positionInFirstSequence, positionInSecondSequence;
  list<RectangleRelativePosition>::iterator itPositionInFirstSequence;
  list<RectangleRelativePosition>::iterator itBestPositionInFirstSequence;

  newRectangleWidth = (*itNewRect)[1][0]-(*itNewRect)[0][0];
  newRectangleHeight = (*itNewRect)[1][1]-(*itNewRect)[0][1];

  for(positionInFirstSequence = 1; positionInFirstSequence<=numberOfPositionnedRectangles+1; ++positionInFirstSequence) {
    for(positionInSecondSequence = 1; positionInSecondSequence<=numberOfPositionnedRectangles+1; ++positionInSecondSequence) {
      newRectangleLeftAbscissa = 0;
      newRectangleLowOrdinate = 0;
      maxWidthOfBoundingBox = 0;
      maxHeightOfBoundingBox = 0;

      itPositionInFirstSequence = testOfPositionOfNewRectangle(positionInFirstSequence,positionInSecondSequence);

      ratio = calculateRatio();
      perimeterOfBoundingRectangle = maxWidthOfBoundingBox+maxHeightOfBoundingBox;

      if( ((ratio<=1.2f)&&((bestRatio>1.2f)||(bestPerimeterOfBoundingRectangle>perimeterOfBoundingRectangle))) || ((1.2f<ratio)&&(bestRatio>ratio) )) {
        bestRatio.value = ratio;
        bestPerimeterOfBoundingRectangle.value = perimeterOfBoundingRectangle;
        bestPlaceInFirstSequence = positionInFirstSequence;
        bestPlaceInSecondSequence = positionInSecondSequence;
        bestWidthOfBoundingBox = maxWidthOfBoundingBox;
        bestHeightOfBoundingBox = maxHeightOfBoundingBox;
        bestRectangleLeftAbscissa = newRectangleLeftAbscissa;
        bestRectangleLowOrdinate = newRectangleLowOrdinate;
        itBestPositionInFirstSequence = itPositionInFirstSequence;
        firstSequence->stockOfTemporaryBestCoordinates(bestPlaceInFirstSequence);
      }
    }
  }

  modificationOfSequencePair(itNewRect, itBestPositionInFirstSequence);
}

list<RectangleRelativePosition>::iterator RectanglePacking::testOfPositionOfNewRectangle(int positionInFirstSequence, int positionInSecondSequence) {

  list<RectangleRelativePosition>::iterator itFirstRectangleRightOrAboveOfNewRectangle;

  if(positionInFirstSequence>=2) {
    itFirstRectangleRightOrAboveOfNewRectangle = positionOfNewRectangle(positionInFirstSequence, positionInSecondSequence);
  }
  else {
    itFirstRectangleRightOrAboveOfNewRectangle = firstSequence->begin();
  }

  dimensionsBoundingBoxOfNewRectangleAndRectanglesLeftOrBelow();

  if(positionInFirstSequence<=numberOfPositionnedRectangles) {
    repositionOfRectanglesRightOrAboveNewRectangle(itFirstRectangleRightOrAboveOfNewRectangle, positionInFirstSequence, positionInSecondSequence);
  }

  return itFirstRectangleRightOrAboveOfNewRectangle;
}


list<RectangleRelativePosition>::iterator RectanglePacking::positionOfNewRectangle(int positionInFirstSequence, int positionInSecondSequence) {

  list<RectangleRelativePosition>::iterator itRectLeftOrBelowOfNewRect = firstSequence->begin();
  int positionRectLeftOrBelowOfNewRect;

  for(positionRectLeftOrBelowOfNewRect = 1; positionRectLeftOrBelowOfNewRect < positionInFirstSequence; ++positionRectLeftOrBelowOfNewRect) {
    coordinatesOfNewRectangle(itRectLeftOrBelowOfNewRect, positionInFirstSequence, positionInSecondSequence);
    dimensionsBoundingBoxOfRectanglesLeftOrBelowNewRectangle(itRectLeftOrBelowOfNewRect);
    ++itRectLeftOrBelowOfNewRect;
  }

  return  itRectLeftOrBelowOfNewRect;
}


void RectanglePacking::repositionOfRectanglesRightOrAboveNewRectangle(list<RectangleRelativePosition>::iterator itFirstRectangleRightOrAboveOfNewRectangle, int positionInFirstSequence, int positionInSecondSequence) {

  list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition;
  int positionRectangleToReposition = positionInFirstSequence;

  for(itRectOfSequenceToReposition = itFirstRectangleRightOrAboveOfNewRectangle; itRectOfSequenceToReposition != firstSequence->end(); ++itRectOfSequenceToReposition) {
    itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa = itRectOfSequenceToReposition->rectangleLeftAbscissa;
    itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate = itRectOfSequenceToReposition->rectangleLowOrdinate;

    modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionNewRectangle(itRectOfSequenceToReposition, positionInSecondSequence);

    if(positionInFirstSequence<positionRectangleToReposition) {
      modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionAlreadyRepositionnedRectangles(itFirstRectangleRightOrAboveOfNewRectangle, itRectOfSequenceToReposition);
    }

    dimensionsBoundingBoxOfAllOptimalPositionnedRectangles(itRectOfSequenceToReposition);

    ++positionRectangleToReposition;
  }
}

void RectanglePacking::modificationOfSequencePair(vector<Rectangle<float> >::iterator itNewRect, list<RectangleRelativePosition>::iterator itBestPositionInFirstSequence) {

  list<RectangleRelativePosition>::iterator itRectToReposition;

  for(int k=0; k<numberOfPositionnedRectangles; ++k) {
    if(placesOfRectanglesInSecondSequence[k] >= bestPlaceInSecondSequence) {
      ++placesOfRectanglesInSecondSequence[k];
    }
  }

  placesOfRectanglesInSecondSequence[numberOfPositionnedRectangles] = bestPlaceInSecondSequence;

  ++numberOfPositionnedRectangles;

  firstSequence->addRectangleRelativePosition(itNewRect, numberOfPositionnedRectangles , newRectangleWidth, newRectangleHeight, bestRectangleLeftAbscissa, bestRectangleLowOrdinate, itBestPositionInFirstSequence);

  if(bestPlaceInFirstSequence < numberOfPositionnedRectangles) {
    for(itRectToReposition=itBestPositionInFirstSequence; itRectToReposition!=firstSequence->end(); ++itRectToReposition) {
      itRectToReposition->rectangleLeftAbscissa = itRectToReposition->rectangleTemporaryBestLeftAbscissa;
      itRectToReposition->rectangleLowOrdinate = itRectToReposition->rectangleTemporaryBestLowOrdinate;
    }
  }
}

float RectanglePacking::calculateRatio() {

  if(maxHeightOfBoundingBox > maxWidthOfBoundingBox) {
    return(maxHeightOfBoundingBox/maxWidthOfBoundingBox);
  }

  else {
    return(maxWidthOfBoundingBox/maxHeightOfBoundingBox);
  }
}


void RectanglePacking::coordinatesOfNewRectangle(list<RectangleRelativePosition>::iterator itRectLeftOrBelowOfNewRect, int, int positionInSecondSequence) {
  if(placesOfRectanglesInSecondSequence[itRectLeftOrBelowOfNewRect->rectangleNumber-1] >= positionInSecondSequence) {
    float highOrdinate;

    if((highOrdinate = itRectLeftOrBelowOfNewRect->rectangleLowOrdinate + itRectLeftOrBelowOfNewRect->rectangleHeight) > newRectangleLowOrdinate) {
      newRectangleLowOrdinate = highOrdinate ;
    }
  }
  else {
    float rightAbscissa;

    if((rightAbscissa= itRectLeftOrBelowOfNewRect->rectangleLeftAbscissa + itRectLeftOrBelowOfNewRect->rectangleWidth) > newRectangleLeftAbscissa) {
      newRectangleLeftAbscissa = rightAbscissa;
    }
  }
}

void RectanglePacking::dimensionsBoundingBoxOfRectanglesLeftOrBelowNewRectangle(list<RectangleRelativePosition>::iterator itRectLeftOrBelowOfNewRect) {

  float rightAbscissa;
  float highOrdinate;

  if((highOrdinate = itRectLeftOrBelowOfNewRect->rectangleLowOrdinate + itRectLeftOrBelowOfNewRect->rectangleHeight) > maxHeightOfBoundingBox) {
    maxHeightOfBoundingBox = highOrdinate;
  }

  if((rightAbscissa = itRectLeftOrBelowOfNewRect->rectangleLeftAbscissa + itRectLeftOrBelowOfNewRect->rectangleWidth) > maxWidthOfBoundingBox) {
    maxWidthOfBoundingBox = rightAbscissa;
  }
}

void RectanglePacking::dimensionsBoundingBoxOfNewRectangleAndRectanglesLeftOrBelow() {

  float rightAbscissa;
  float highOrdinate;

  if((rightAbscissa = newRectangleWidth + newRectangleLeftAbscissa) > maxWidthOfBoundingBox) {
    maxWidthOfBoundingBox = rightAbscissa;
  }

  if((highOrdinate = newRectangleHeight + newRectangleLowOrdinate) > maxHeightOfBoundingBox) {
    maxHeightOfBoundingBox = highOrdinate;
  }
}


void RectanglePacking::modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionNewRectangle(list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition, int positionInSecondSequence) {
  if(placesOfRectanglesInSecondSequence[itRectOfSequenceToReposition->rectangleNumber-1] < positionInSecondSequence ) {
    float highOrdinate;

    if((highOrdinate = newRectangleLowOrdinate + newRectangleHeight) > itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate) {
      itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate = highOrdinate;
    }
  }
  else {
    float rightAbscissa;

    if((rightAbscissa = newRectangleLeftAbscissa + newRectangleWidth) > itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa) {
      itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa = rightAbscissa;
    }
  }
}


void RectanglePacking::modificationCoordinatesRectanglesRightOrAboveNewRectangleInFonctionAlreadyRepositionnedRectangles(list<RectangleRelativePosition>::iterator itFirstRectangleRightOrAboveOfNewRectangle, list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition) {

  list<RectangleRelativePosition>::iterator itRectOfSequenceAlreadyRepositionned;
  float rightAbscissa;
  float highOrdinate;

  for(itRectOfSequenceAlreadyRepositionned=itFirstRectangleRightOrAboveOfNewRectangle; itRectOfSequenceAlreadyRepositionned!=itRectOfSequenceToReposition; ++itRectOfSequenceAlreadyRepositionned) {
    if(placesOfRectanglesInSecondSequence[itRectOfSequenceAlreadyRepositionned->rectangleNumber-1]  >=  placesOfRectanglesInSecondSequence[itRectOfSequenceToReposition->rectangleNumber-1]) {
      if((highOrdinate = itRectOfSequenceAlreadyRepositionned->rectangleTemporaryLowOrdinate + itRectOfSequenceAlreadyRepositionned->rectangleHeight) > itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate) {
        itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate = highOrdinate;
      }
    }
    else {
      if((rightAbscissa = itRectOfSequenceAlreadyRepositionned->rectangleTemporaryLeftAbscissa + itRectOfSequenceAlreadyRepositionned->rectangleWidth) > itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa) {
        itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa = rightAbscissa;
      }
    }
  }
}

void RectanglePacking::dimensionsBoundingBoxOfAllOptimalPositionnedRectangles(list<RectangleRelativePosition>::iterator itRectOfSequenceToReposition) {

  float rightAbscissa;
  float highOrdinate;

  if((rightAbscissa = itRectOfSequenceToReposition->rectangleTemporaryLeftAbscissa + itRectOfSequenceToReposition->rectangleWidth) > maxWidthOfBoundingBox) {
    maxWidthOfBoundingBox = rightAbscissa ;
  }

  if((highOrdinate = itRectOfSequenceToReposition->rectangleTemporaryLowOrdinate + itRectOfSequenceToReposition->rectangleHeight) > maxHeightOfBoundingBox) {
    maxHeightOfBoundingBox = highOrdinate;
  }
}


int RectanglePacking::calculOfNumberOptimalRepositionnedRectangles(const char * quality) {

  if(!strcmp(quality,"n5"))
    return numberOfRectangles;

  else if(!strcmp(quality,"n4logn"))
    return (int)floor(pow((double)pow((float)numberOfRectangles,4)*log((float)numberOfRectangles),0.2));

  else if(!strcmp(quality,"n4"))
    return (int)floor(pow((double)numberOfRectangles,0.8));

  else if(!strcmp(quality,"n3logn"))
    return (int)floor(pow((double)pow((float)numberOfRectangles,3)*log((float)numberOfRectangles),0.2));

  else if(!strcmp(quality,"n3"))
    return (int)floor(pow((double)numberOfRectangles,0.6));

  else if(!strcmp(quality,"n2logn"))
    return (int)floor(pow((double)pow((float)numberOfRectangles,2)*log((float)numberOfRectangles),0.2));

  else if(!strcmp(quality,"n2"))
    return (int)floor(pow((double)numberOfRectangles,0.4));

  else if(!strcmp(quality,"nlogn"))
    return (int)floor(pow((double)numberOfRectangles*log((float)numberOfRectangles),0.2));

  else if(!strcmp(quality,"n"))
    return (int)floor(pow((double)numberOfRectangles,0.2));

  return 0;
}


void RectanglePacking::defaultPositionRestOfRectangles(vector<Rectangle<float> >::iterator itlim, vector<Rectangle<float> >::iterator itend) {

  vector<Rectangle<float> >::iterator itr;
  bool boolWidth = false;
  bool boolHeight = false;
  float heightTemp = 0;
  float widthTemp = 0;

  lineOrColumnToStart(boolWidth, boolHeight);

  for(itr=itlim; itr!=itend; ++itr) {
    newRectangleWidth = (*itr)[1][0] - (*itr)[0][0];
    newRectangleHeight = (*itr)[1][1] - (*itr)[0][1];

    if((boolWidth) && (widthTemp>0) && (widthTemp+newRectangleWidth>bestWidthOfBoundingBox)) {
      endOfLine(heightTemp, widthTemp, boolWidth, boolHeight);
    }

    if((boolHeight) && (heightTemp>0) && (heightTemp+newRectangleHeight>bestHeightOfBoundingBox)) {
      endOfColumn(widthTemp, heightTemp, boolWidth, boolHeight);
    }

    if(boolWidth) {
      continueLine(itr, widthTemp, heightTemp, boolWidth, boolHeight);
      continue;
    }

    if(boolHeight) {
      continueColumn(itr, widthTemp, heightTemp, boolWidth, boolHeight);
      continue;
    }
  }

  /* to include if we want to know the final values of the dimensions of the including rectangle. */
  /*
  if((boolWidth)&&(widthTemp!=0)&&(heightTemp!=0)){
    bestHeightOfBoundingBox+=heightTemp;
  }

  if((boolHeight)&&(widthTemp!=0)&&(heightTemp!=0)){
    bestWidthOfBoundingBox+=widthTemp;
  }
  */
}

void RectanglePacking::lineOrColumnToStart(bool & boolWidth, bool & boolHeight) {

  float ratio;

  if(bestHeightOfBoundingBox>bestWidthOfBoundingBox)
    ratio = bestHeightOfBoundingBox/bestWidthOfBoundingBox;
  else
    ratio = bestWidthOfBoundingBox/bestHeightOfBoundingBox;

  if((ratio>1.1) && (bestHeightOfBoundingBox>=bestWidthOfBoundingBox)) {
    boolWidth = 0;
    boolHeight = 1;
  }
  else if((ratio>1.1) && (bestHeightOfBoundingBox<bestWidthOfBoundingBox)) {
    boolWidth = 1;
    boolHeight = 0;
  }
  else if(bestHeightOfBoundingBox>=bestWidthOfBoundingBox) {
    boolWidth = 0;
    boolHeight = 1;
  }
  else if(bestHeightOfBoundingBox<bestWidthOfBoundingBox) {
    boolWidth = 1;
    boolHeight = 0;
  }
}

void RectanglePacking::endOfLine(float & heightTemp, float & widthTemp, bool & boolWidth, bool & boolHeight) {

  float ratio;

  bestHeightOfBoundingBox += heightTemp;
  heightTemp = 0;
  widthTemp = 0;

  if(bestHeightOfBoundingBox>bestWidthOfBoundingBox)
    ratio = bestHeightOfBoundingBox/bestWidthOfBoundingBox;
  else
    ratio = bestWidthOfBoundingBox/bestHeightOfBoundingBox;

  if((ratio>1.1) && (bestWidthOfBoundingBox>bestHeightOfBoundingBox)) {
    boolWidth = 1;
    boolHeight = 0;
  }
  else {
    boolWidth = 0;
    boolHeight = 1;
  }
}

void RectanglePacking::endOfColumn(float & widthTemp, float & heightTemp, bool & boolWidth, bool & boolHeight) {

  float ratio;

  bestWidthOfBoundingBox += widthTemp;
  widthTemp = 0;
  heightTemp = 0;

  if(bestHeightOfBoundingBox>bestWidthOfBoundingBox)
    ratio = bestHeightOfBoundingBox/bestWidthOfBoundingBox;
  else
    ratio = bestWidthOfBoundingBox/bestHeightOfBoundingBox;

  if((ratio>1.1) && (bestHeightOfBoundingBox>bestWidthOfBoundingBox)) {
    boolWidth = 0;
    boolHeight = 1;
  }
  else {
    boolWidth = 1;
    boolHeight = 0;
  }
}

void RectanglePacking::continueLine(vector<Rectangle<float> >::iterator itr, float & widthTemp,float & heightTemp, bool & boolWidth, bool & boolHeight) {
  if((widthTemp==0) && (newRectangleWidth>bestWidthOfBoundingBox)) {
    (*itr)[0][0] = 0;
    (*itr)[1][0] = newRectangleWidth;
    (*itr)[0][1] = bestHeightOfBoundingBox;
    (*itr)[1][1] = bestHeightOfBoundingBox+newRectangleHeight;
    bestWidthOfBoundingBox = newRectangleWidth;
    bestHeightOfBoundingBox += newRectangleHeight;

    float ratio;

    if(bestHeightOfBoundingBox>bestWidthOfBoundingBox)
      ratio = bestHeightOfBoundingBox/bestWidthOfBoundingBox;
    else
      ratio = bestWidthOfBoundingBox/bestHeightOfBoundingBox;

    if((ratio>1.1) && (bestWidthOfBoundingBox>bestHeightOfBoundingBox)) {
      boolWidth = 1;
      boolHeight = 0;
    }
    else {
      boolWidth = 0;
      boolHeight = 1;
    }
  }

  else {
    (*itr)[0][0] = widthTemp;
    (*itr)[1][0] = widthTemp+newRectangleWidth;
    (*itr)[0][1] = bestHeightOfBoundingBox;
    (*itr)[1][1] = bestHeightOfBoundingBox+newRectangleHeight;
    widthTemp += newRectangleWidth;

    if(newRectangleHeight>heightTemp)
      heightTemp = newRectangleHeight;
  }
}

void RectanglePacking::continueColumn(vector<Rectangle<float> >::iterator itr, float & widthTemp,float & heightTemp, bool & boolWidth, bool & boolHeight) {
  if((heightTemp==0) && (newRectangleHeight>bestHeightOfBoundingBox)) {
    (*itr)[0][0] = bestWidthOfBoundingBox;
    (*itr)[1][0] = bestWidthOfBoundingBox+newRectangleWidth;
    (*itr)[0][1] = 0;
    (*itr)[1][1] = newRectangleHeight;
    bestHeightOfBoundingBox = newRectangleHeight;
    bestWidthOfBoundingBox += newRectangleWidth;

    float ratio;

    if(bestHeightOfBoundingBox>bestWidthOfBoundingBox)
      ratio = bestHeightOfBoundingBox/bestWidthOfBoundingBox;
    else
      ratio = bestWidthOfBoundingBox/bestHeightOfBoundingBox;

    if((ratio>1.1) && (bestHeightOfBoundingBox>bestWidthOfBoundingBox)) {
      boolWidth = 0;
      boolHeight = 1;
    }
    else {
      boolWidth = 1;
      boolHeight = 0;
    }
  }
  else {
    (*itr)[0][0] = bestWidthOfBoundingBox;
    (*itr)[1][0] = bestWidthOfBoundingBox+newRectangleWidth;
    (*itr)[0][1] = heightTemp;
    (*itr)[1][1] = heightTemp+newRectangleHeight;
    heightTemp += newRectangleHeight;

    if(newRectangleWidth>widthTemp)
      widthTemp = newRectangleWidth;

  }
}

void RectanglePacking::optimalPositionOfNewRectangleLimPos(vector<Rectangle<float> >::iterator itNewRect, int numberTestedPositions) {
  float ratio;
  Number bestRatio(Number::infini);
  float perimeterOfBoundingRectangle;
  Number bestPerimeterOfBoundingRectangle(Number::infini);
  int positionInFirstSequence, positionInSecondSequence;
  list<RectangleRelativePosition>::iterator itPositionInFirstSequence;
  list<RectangleRelativePosition>::iterator itBestPositionInFirstSequence;
  int numberOfLastTestedPosition;
  vector<bool> testedPositions(numberOfPositionnedRectangles+1);

  newRectangleLeftAbscissa = 0;
  newRectangleLowOrdinate = 0;
  maxHeightOfBoundingBox = 0;
  maxWidthOfBoundingBox = 0;

  newRectangleWidth = (*itNewRect)[1][0]-(*itNewRect)[0][0];
  newRectangleHeight = (*itNewRect)[1][1]-(*itNewRect)[0][1];

  if(numberOfPositionnedRectangles >= numberTestedPositions) {
    numberOfLastTestedPosition=numberOfPositionnedRectangles -  numberTestedPositions + 2;
    int quotient = (numberOfPositionnedRectangles+1)/numberTestedPositions;

    for(int i=0; i<=numberTestedPositions-1; ++i) {
      testedPositions[i*quotient] = true;

      for(int j=i*quotient+1; j<=(i+1)*quotient-1; ++j) {
        testedPositions[j] = false;
      }
    }

    for(int i=numberTestedPositions*quotient; i<=numberOfPositionnedRectangles; ++i) {
      testedPositions[i] = false;
    }
  }
  else {
    numberOfLastTestedPosition=1;
  }

  for(positionInFirstSequence = numberOfPositionnedRectangles+1; positionInFirstSequence>=numberOfLastTestedPosition; positionInFirstSequence--) {
    for(positionInSecondSequence = 1; positionInSecondSequence<=numberOfPositionnedRectangles+1; ++positionInSecondSequence) {
      if((testedPositions[positionInSecondSequence-1])||(numberOfLastTestedPosition==1 )) {
        itPositionInFirstSequence = testOfPositionOfNewRectangle(positionInFirstSequence, positionInSecondSequence);
        ratio = calculateRatio();
        perimeterOfBoundingRectangle = maxWidthOfBoundingBox+maxHeightOfBoundingBox;

        if( ((ratio<=1.2f)&&((bestRatio>1.2f)||(bestPerimeterOfBoundingRectangle>perimeterOfBoundingRectangle))) || ((1.2f<ratio)&&(bestRatio>ratio) )) {
          bestRatio.value = ratio;
          bestPerimeterOfBoundingRectangle.value = perimeterOfBoundingRectangle;
          bestPlaceInFirstSequence = positionInFirstSequence;
          bestPlaceInSecondSequence = positionInSecondSequence;
          bestWidthOfBoundingBox = maxWidthOfBoundingBox;
          bestHeightOfBoundingBox = maxHeightOfBoundingBox;
          bestRectangleLeftAbscissa = newRectangleLeftAbscissa;
          bestRectangleLowOrdinate = newRectangleLowOrdinate;
          itBestPositionInFirstSequence = itPositionInFirstSequence;
          firstSequence->stockOfTemporaryBestCoordinates(bestPlaceInFirstSequence);
        }

        newRectangleLeftAbscissa = 0;
        newRectangleLowOrdinate = 0;
        maxWidthOfBoundingBox = 0;
        maxHeightOfBoundingBox = 0;
      }
    }
  }

  modificationOfSequencePair(itNewRect, itBestPositionInFirstSequence);
}


int RectanglePacking::calculNumberOfTestedPositions(const char * quality) {

  if(!strcmp(quality,"n5"))
    return numberOfRectangles;

  else if(!strcmp(quality,"n4logn"))
    return (int)floor(pow((double)pow((float)numberOfRectangles,3)*log((float)numberOfRectangles),0.25));

  else if(!strcmp(quality,"n4"))
    return (int)floor(pow((double)numberOfRectangles,0.75));

  else if(!strcmp(quality,"n3logn"))
    return (int)floor(pow((double)pow((float)numberOfRectangles,2)*log((float)numberOfRectangles),0.25));

  else if(!strcmp(quality,"n3"))
    return (int)floor(sqrt((double)numberOfRectangles));

  else if(!strcmp(quality,"n2logn"))
    return (int)floor(sqrt((double)log((float)numberOfRectangles)));

  return 0;
}
