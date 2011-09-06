/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef SIZEPROPERTYANIMATION_H_
#define SIZEPROPERTYANIMATION_H_

#include "VectorPropertyAnimation.h"
#include <tulip/SizeProperty.h>

class SizePropertyAnimation: public VectorPropertyAnimation<tlp::SizeProperty, tlp::Size, float, 3> {
public:
  SizePropertyAnimation(tlp::Graph *graph, tlp::SizeProperty *start, tlp::SizeProperty *end, tlp::SizeProperty *out,
      tlp::BooleanProperty *selection = 0, int frameCount = 1, bool computeNodes = true, bool computeEdges = false);
};

#endif /* SIZEPROPERTYANIMATION_H_ */
