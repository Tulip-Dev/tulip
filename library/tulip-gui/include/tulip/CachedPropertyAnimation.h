#ifndef CACHEDPROPERTYANIMATION_H_
#define CACHEDPROPERTYANIMATION_H_

#include "tulip/PropertyAnimation.h"
#include <map>
#include <tulip/BooleanProperty.h>

namespace tlp {

template<typename PropType, typename NodeType, typename EdgeType>
class CachedPropertyAnimation: public PropertyAnimation<PropType, NodeType, EdgeType> {
public:
  CachedPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out, tlp::BooleanProperty *selection = 0, int frameCount = 1,
                          bool computeNodes = true, bool computeEdges = true);
  virtual ~CachedPropertyAnimation();

  virtual void frameChanged(int frame);

protected:
  std::map<std::pair<NodeType, NodeType>, NodeType> computedNodeSteps;
  std::map<std::pair<EdgeType, EdgeType>, EdgeType> computedEdgeSteps;
};

#include "cxx/CachedPropertyAnimation.cxx"

}

#endif /* CACHEDPROPERTYANIMATION_H_ */
