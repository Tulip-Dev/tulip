/*
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
///@cond DOXYGEN_HIDDEN

#ifndef VECTORPROPERTYANIMATION_H_
#define VECTORPROPERTYANIMATION_H_

#include "tulip/CachedPropertyAnimation.h"

namespace tlp {

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
class VectorPropertyAnimation: public CachedPropertyAnimation<PropType, RealType, RealType> {
public:
  VectorPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out, tlp::BooleanProperty *selection = NULL, int frameCount = 1,
                          bool computeNodes = true, bool computeEdges = true, QObject* parent=NULL);

  virtual ~VectorPropertyAnimation() {}

protected:
  virtual RealType
  getNodeFrameValue(const RealType &startValue, const RealType &endValue, int frame);
  virtual RealType
  getEdgeFrameValue(const RealType &startValue, const RealType &endValue, int frame);

private:
  typedef tlp::Vector<VectorType, SIZE> PropVector;

  std::map<std::pair<tlp::Vector<VectorType, SIZE>, tlp::Vector<VectorType, SIZE> >, tlp::Vector<double, SIZE> > steps;
};

#include "cxx/VectorPropertyAnimation.cxx"

}

#endif /* VECTORPROPERTYANIMATION_H_ */
///@endcond
