#include "tulip/SizePropertyAnimation.h"

SizePropertyAnimation::SizePropertyAnimation(tlp::Graph *graph, tlp::SizeProperty *start, tlp::SizeProperty *end, tlp::SizeProperty *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges) :
  VectorPropertyAnimation<tlp::SizeProperty, tlp::Size, float, 3> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges) {
}
