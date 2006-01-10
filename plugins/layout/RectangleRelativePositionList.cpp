#include "RectangleRelativePositionList.h"



void RectangleRelativePositionList::addRectangleRelativePosition(vector<Rectangle<float> >::iterator itr, int numRect, float wdth, float hght, float x, float y, list<RectangleRelativePosition>::iterator itRectangleRelativePosition){

  RectangleRelativePosition newRectangleRelativePosition;   
  newRectangleRelativePosition.rectangleIterator = itr;
  newRectangleRelativePosition.rectangleNumber = numRect;
  newRectangleRelativePosition.rectangleWidth = wdth;
  newRectangleRelativePosition.rectangleHeight = hght;
  newRectangleRelativePosition.rectangleLeftAbscissa = x;
  newRectangleRelativePosition.rectangleLowOrdinate = y;
  
  this->insert(itRectangleRelativePosition, newRectangleRelativePosition);
}



void RectangleRelativePositionList::allocateCoordinates(){

  for(list<RectangleRelativePosition>::iterator itr=this->begin(); itr!=this->end(); ++itr){

    (*(itr->rectangleIterator))[0][0] = itr->rectangleLeftAbscissa;
    (*(itr->rectangleIterator))[0][1] = itr->rectangleLowOrdinate;
    (*(itr->rectangleIterator))[1][0] = (*(itr->rectangleIterator))[0][0] + itr->rectangleWidth;
    (*(itr->rectangleIterator))[1][1] = (*(itr->rectangleIterator))[0][1]  + itr->rectangleHeight;
  }
}


void RectangleRelativePositionList::stockOfTemporaryBestCoordinates(int bestPlaceInFirstSequence){
  
  list<RectangleRelativePosition>::reverse_iterator itRectToReposition = this->rend();
  int positionRectToReposition;


  for(positionRectToReposition = this->size(); positionRectToReposition>=bestPlaceInFirstSequence; --positionRectToReposition){
    itRectToReposition++; 
    itRectToReposition->rectangleTemporaryBestLeftAbscissa = itRectToReposition->rectangleTemporaryLeftAbscissa;
    itRectToReposition->rectangleTemporaryBestLowOrdinate = itRectToReposition->rectangleTemporaryLowOrdinate;
    }

}
