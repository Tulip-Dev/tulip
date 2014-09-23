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
std::vector<tlp::Rectangle<float> > & RectanglePackingLimitRectangles(std::vector<tlp::Rectangle<float> > & v, const char * quality, tlp::PluginProgress * progress);

/**
 *This function is used for packing rectangles. It is used when we want to
 *reduce the complexity restricting the number of tested positions for each
 *rectangle to pack.
 *@param v a vector including the rectangles to pack.
 *@param quality give the complexity of the algorithm.
 *@param progress pointer on an instance of the PluginProgress class.
 *@return the vector v whose rectangles have had their co-ordinates changed.
 */
std::vector<tlp::Rectangle<float> > & RectanglePackingLimitPositions(std::vector<tlp::Rectangle<float> > & v, const char * quality, tlp::PluginProgress * progress);

#endif
