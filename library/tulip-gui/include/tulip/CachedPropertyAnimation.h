/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef CACHEDPROPERTYANIMATION_H_
#define CACHEDPROPERTYANIMATION_H_

#include <tulip/PropertyAnimation.h>

#include <map>

namespace tlp {

class BooleanProperty;

template <typename PropType, typename NodeType, typename EdgeType>
class CachedPropertyAnimation : public PropertyAnimation<PropType, NodeType, EdgeType> {
public:
  CachedPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
                          tlp::BooleanProperty *selection = nullptr, int frameCount = 1,
                          bool computeNodes = true, bool computeEdges = true,
                          QObject *parent = nullptr);
  ~CachedPropertyAnimation() override;

  void frameChanged(int frame) override;

protected:
  std::map<std::pair<NodeType, NodeType>, NodeType> computedNodeSteps;
  std::map<std::pair<EdgeType, EdgeType>, EdgeType> computedEdgeSteps;
};

#include "cxx/CachedPropertyAnimation.cxx"
}

#endif /* CACHEDPROPERTYANIMATION_H_ */
///@endcond
