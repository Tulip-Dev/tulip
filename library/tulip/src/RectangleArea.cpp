#include "tulip/RectangleArea.h"

RectangleArea::RectangleArea(float x, float y, float width, float height) {
    direction = HORIZONTAL_DIRECTION;
    set2D(x, y, width, height);
}

//====================================================================
RectangleArea::RectangleArea(const Coord& coord, const Size& size) {
    set2D(coord.getX(), coord.getY(), size.getW(), size.getH());
}

//====================================================================
RectangleArea::RectangleArea(const Size& size) {
    set2D(0.f, 0.f, size.getW(), size.getH());
}

//====================================================================
RectangleArea::RectangleArea(const Coord& coord) {
    set2D(coord.getX(), coord.getY(), 0.f, 0.f);
}

//====================================================================
void RectangleArea::set2D(float xParam, float yParam, float widthParam,
                          float heightParam) {
    x      = xParam;
    y      = yParam;
    width  = widthParam;
    height = heightParam;
}


//====================================================================
void RectangleArea::setDirection(directionType directionParam) {
    direction = directionParam;
}

//====================================================================
directionType RectangleArea::getDirection() const {
    return direction;
}

//====================================================================
float& RectangleArea::getX() {
    return x;
}

//====================================================================
float& RectangleArea::getY() {
    return y;
}

//====================================================================
float& RectangleArea::getVirtualX() {
    return (direction == HORIZONTAL_DIRECTION ? x : y);
}

//====================================================================
float& RectangleArea::getVirtualY() {
    return (direction == HORIZONTAL_DIRECTION ? y : x);
}

//====================================================================
float& RectangleArea::getWidth() {
    return width;
}

//====================================================================
float& RectangleArea::getHeight() {
    return height;
}

//====================================================================
float& RectangleArea::getVirtualWidth() {
    return (direction == HORIZONTAL_DIRECTION ? width : height);
}

//====================================================================
float& RectangleArea::getVirtualHeight() {
    return (direction == HORIZONTAL_DIRECTION ? height : width);
}   

//====================================================================
float RectangleArea::getX() const {
    return x;
}

//====================================================================
float RectangleArea::getY() const {
    return y;
}
//====================================================================
float RectangleArea::getWidth() const {
    return width;
}

//====================================================================
float RectangleArea::getHeight() const {
    return height;
}

//====================================================================
float RectangleArea::getVirtualX() const {
    return (direction == HORIZONTAL_DIRECTION ? x : y);
}

//====================================================================
float RectangleArea::getVirtualY() const {
    return (direction == HORIZONTAL_DIRECTION ? y : x);
}

//====================================================================
float RectangleArea::getVirtualWidth() const {
    return (direction == HORIZONTAL_DIRECTION ? width : height);
}

//====================================================================
float RectangleArea::getVirtualHeight() const {
    return (direction == HORIZONTAL_DIRECTION ? height : width);
}   

//====================================================================>
Coord RectangleArea::getCenterCoord() const {
    return Coord(x + width / 2.f, y + height / 2.f, 0.f);
}

//====================================================================
Coord RectangleArea::getCoord() const {
    return Coord(x, y, 0.f);
}

//====================================================================
Size RectangleArea::getSize() const {
    return Size(width, height, 1.f);
}

//====================================================================
float RectangleArea::getArea() const {
    return  width * height;
}

//====================================================================
Coord RectangleArea::getCoord2() const {
    return Coord( getX() + getWidth(),
                  getY() + getHeight());   
}
    
    
//====================================================================
bool intersect(const RectangleArea& rect1, const RectangleArea& rect2) {
    
    const Coord rect1Pts1 = rect1.getCoord();    
    const Coord rect2Pts1 = rect2.getCoord();
    const Coord rect1Pts2 = rect1.getCoord2();
    const Coord rect2Pts2 = rect2.getCoord2();
        
    return rect1Pts1.getX()<rect2Pts2.getX() && 
           rect1Pts2.getX()>rect2Pts1.getX() &&
           rect1Pts1.getY()<rect2Pts2.getY() &&
           rect1Pts2.getY()>rect2Pts1.getY();
           
}


