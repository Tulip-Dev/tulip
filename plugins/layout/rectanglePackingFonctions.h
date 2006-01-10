#ifndef RECTANGLEPACKINGFONCTIONS_H
#define RECTANGLEPACKINGFONCTIONS_H

#include <vector>
#include "RectanglePacking.h"


/**
 *This function is used for packing rectangles. It is used when we want to 
 *reduce the complexity restricting the number of rectangles packed in an 
 *optimal way.
 *@param v a vector including the rectangles to pack.
 *@param quality give the complexity of the algorithm.
 *@param progress pointer on an instance of the PluginProgress class.
 *@return the vector v whose rectangles have had their co-ordinates changed.
 */
vector<Rectangle<float> > & RectanglePackingLimitRectangles(vector<Rectangle<float> > & v, const char * quality, PluginProgress * progress);
  

/**
 *This function is used for packing rectangles. It is used when we want to 
 *reduce the complexity restricting the number of tested positions for each 
 *rectangle to pack.
 *@param v a vector including the rectangles to pack.
 *@param quality give the complexity of the algorithm.
 *@param progress pointer on an instance of the PluginProgress class.
 *@return the vector v whose rectangles have had their co-ordinates changed.
 */
vector<Rectangle<float> > & RectanglePackingLimitPositions(vector<Rectangle<float> > & v, const char * quality, PluginProgress * progress);


#endif
