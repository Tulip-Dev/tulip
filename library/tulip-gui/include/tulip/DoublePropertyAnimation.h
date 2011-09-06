#ifndef DOUBLEPROPERTYANIMATION_H
#define DOUBLEPROPERTYANIMATION_H

#include "tulip/CachedPropertyAnimation.h"
#include <tulip/DoubleProperty.h>

namespace tlp {

class DoublePropertyAnimation:public CachedPropertyAnimation<tlp::DoubleProperty, double, double> {
public:
    DoublePropertyAnimation(tlp::Graph *graph, tlp::DoubleProperty *start, tlp::DoubleProperty *end, tlp::DoubleProperty *out,
        tlp::BooleanProperty *selection = 0, int frameCount = 1, bool computeNodes = true, bool computeEdges = false);
    ~DoublePropertyAnimation(){}

protected:
    double getNodeFrameValue(const double &startValue, const double &endValue, int frame);
    double getEdgeFrameValue(const double &startValue, const double &endValue, int frame);
};

}

#endif // DOUBLEPROPERTYANIMATION_H
