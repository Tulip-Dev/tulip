#ifndef VECTORPROPERTYANIMATION_H_
#define VECTORPROPERTYANIMATION_H_

#include "tulip/CachedPropertyAnimation.h"

namespace tlp {

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
class VectorPropertyAnimation: public CachedPropertyAnimation<PropType, RealType, RealType> {
public:
  VectorPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end, PropType *out, tlp::BooleanProperty *selection = 0, int frameCount = 1,
                          bool computeNodes = true, bool computeEdges = true);

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
