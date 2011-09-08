#ifndef PROPERTYANIMATION_H_
#define PROPERTYANIMATION_H_

#include "tulip/Animation.h"
#include <tulip/BooleanProperty.h>


namespace tlp {

template<typename PropType, typename NodeType, typename EdgeType>
class TLP_QT_SCOPE PropertyAnimation: public Animation {
public:
  PropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out,
                    tlp::BooleanProperty *selection = 0, int frameCount = 1, bool computeNodes = true, bool computeEdges = true, QObject* parent=0);

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
