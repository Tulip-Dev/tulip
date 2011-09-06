#include "tulip/DoublePropertyAnimation.h"

using namespace std;
using namespace tlp;

DoublePropertyAnimation::DoublePropertyAnimation(Graph *graph, DoubleProperty *start, DoubleProperty *end, DoubleProperty *out,BooleanProperty *selection , int frameCount , bool computeNodes , bool computeEdges ):CachedPropertyAnimation<DoubleProperty, double, double>(graph,start,end,out,selection,frameCount,computeNodes,computeEdges){

}

double DoublePropertyAnimation::getNodeFrameValue(const double &startValue, const double &endValue, int frame){
    if(frame != 0){
       return startValue + ((endValue-startValue)/(frameCount()))*frame;
    }
    else {
      return startValue;
    }
}

double DoublePropertyAnimation::getEdgeFrameValue(const double &startValue, const double &endValue, int frame){
    if(frame != 0){
      return startValue + ((endValue-startValue)/(frameCount()))*frame;
    }else{
        return startValue;
    }
}
