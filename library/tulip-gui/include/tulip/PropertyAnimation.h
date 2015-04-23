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

#ifndef PROPERTYANIMATION_H_
#define PROPERTYANIMATION_H_

#include <tulip/Animation.h>
#include <tulip/BooleanProperty.h>


namespace tlp {

template<typename PropType, typename NodeType, typename EdgeType>
class PropertyAnimation: public Animation {
public:
  PropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
                    tlp::BooleanProperty *selection = NULL, int frameCount = 1, bool computeNodes = true, bool computeEdges = true, QObject* parent=NULL);

  virtual ~PropertyAnimation();
  virtual void frameChanged(int f);

protected:
  tlp::Graph *_graph;
  PropType *_start;
  PropType *_end;
  PropType *_out;
  tlp::BooleanProperty *_selection;
  bool _computeNodes;
  bool _computeEdges;

  virtual NodeType getNodeFrameValue(const NodeType &startValue, const NodeType &endValue, int frame)=0;
  virtual EdgeType getEdgeFrameValue(const EdgeType &startValue, const EdgeType &endValue, int frame)=0;

  virtual bool equalNodes(const NodeType &v1, const NodeType &v2) {
    return v1 == v2;
  }

  virtual bool equalEdges(const EdgeType &v1, const EdgeType &v2) {
    return v1 == v2;
  }
};

#include "cxx/PropertyAnimation.cxx"

}

#endif /* PROPERTYANIMATION_H_ */
///@endcond
