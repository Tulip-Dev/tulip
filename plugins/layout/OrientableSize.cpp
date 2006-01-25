#include "OrientableSize.h"
#include "OrientableSizeProxy.h"


//====================================================================
OrientableSize::OrientableSize(OrientableSizeProxy* fatherParam,
                               const float width, const float height,
                               const float depth)
    : father(fatherParam) {
    set(width, height, depth);
}

//====================================================================
OrientableSize::OrientableSize(OrientableSizeProxy* fatherParam,
                               const Size& size)
    :father(fatherParam) {
    set(size);
}

//====================================================================
void OrientableSize::set(const float width, const float height,
                         const float depth) {
    setW(width);
    setH(height);
    setD(depth);
}

//====================================================================
void OrientableSize::set(const Size& size) {
    Size::setW(size.getW());
    Size::setH(size.getH());
    Size::setD(size.getD());
}

//====================================================================
void OrientableSize::get(float* width, float* height, float* depth) const {
    *width  = getW();
    *height = getH();
    *depth  = getD();
}

//====================================================================
void OrientableSize::setW(float w) {
    (this->*(father->writeW)) (w);
}

//====================================================================
void OrientableSize::setH(float h) {
    (this->*(father->writeH)) (h);
}

//====================================================================
void OrientableSize::setD(float d) {
    (this->*(father->writeD)) (d);
}

//====================================================================
float OrientableSize::getW() const {
    return (this->*(father->readW)) ();
}

//====================================================================
float OrientableSize:: getH() const {
    return (this->*(father->readH)) ();
}

//====================================================================
float OrientableSize::getD() const {
    return (this->*(father->readD)) ();
}
