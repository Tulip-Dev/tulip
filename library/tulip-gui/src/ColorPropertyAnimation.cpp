#include "tulip/ColorPropertyAnimation.h"

ColorPropertyAnimation::ColorPropertyAnimation(tlp::Graph *graph, tlp::ColorProperty *start, tlp::ColorProperty *end, tlp::ColorProperty *out,
    tlp::BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges) :
  VectorPropertyAnimation<tlp::ColorProperty, tlp::Color, unsigned char, 4> (graph, start, end, out, selection, frameCount, computeNodes, computeEdges) {
}
