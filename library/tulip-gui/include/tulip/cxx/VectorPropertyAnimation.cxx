#include <tulip/ForEach.h>

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::VectorPropertyAnimation(tlp::Graph *graph, PropType *start, PropType *end,
    PropType *out, tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges) :
  CachedPropertyAnimation<PropType, RealType, RealType> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges) {

  assert(frameCount > 1);

  if (this->computeNodes) {
    tlp::node n;
    forEach(n, this->graph->getNodes()) {
      if (this->selection && !this->selection->getNodeValue(n))
        continue;

      std::pair<PropVector, PropVector> values(PropVector(this->start->getNodeValue(n)), PropVector(this->end->getNodeValue(n)));
      if (steps.find(values) == steps.end()) {
        tlp::Vector<double, SIZE> stepsVector;
        for (unsigned int i = 0; i < SIZE; ++i)
          stepsVector[i] = ((double)values.second[i] - (double)values.first[i]) * 1. / (frameCount - 1);
        steps[values] = stepsVector;
      }
    }
  }

  if (this->computeEdges) {
    tlp::edge e;
    forEach(e, this->graph->getEdges()) {
      if (this->selection && !this->selection->getEdgeValue(e))
        continue;
      std::pair<PropVector, PropVector> values(PropVector(this->start->getEdgeValue(e)), PropVector(this->end->getEdgeValue(e)));
      if (steps.find(values) == steps.end()) {
        tlp::Vector<double, SIZE> stepsVector;
        for (unsigned int i = 0; i < SIZE; ++i)
          stepsVector[i] = ((double) (values.second[i] - (double)values.first[i])) * 1. / (frameCount - 1);
        steps[values] = stepsVector;
      }
    }
  }
}

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
RealType VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::getNodeFrameValue(const RealType &startValue, const RealType &endValue,
    int frame) {
  PropVector result((PropVector)startValue);
  std::pair<PropVector, PropVector> values(result, (PropVector) endValue);
  tlp::Vector<double, SIZE> stepVal(steps[values]);
  for (unsigned i=0; i < SIZE; ++i)
    result[i] += stepVal[i] * frame;
  return RealType(result);
}

template<typename PropType, typename RealType, typename VectorType, unsigned int SIZE>
RealType VectorPropertyAnimation<PropType, RealType, VectorType, SIZE>::getEdgeFrameValue(const RealType &startValue, const RealType &endValue,
    int frame) {
  return getNodeFrameValue(startValue, endValue, frame);
}
