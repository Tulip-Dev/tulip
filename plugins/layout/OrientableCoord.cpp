#include "OrientableLayout.h"
#include "OrientableCoord.h"


//====================================================================
OrientableCoord::OrientableCoord(OrientableLayout* fatherParam, const float x,
                                 const float y, const float z)
    :father(fatherParam) {
    set(x, y, z);
}
//====================================================================
OrientableCoord::OrientableCoord(OrientableLayout* fatherParam, const Coord& v)
    :father(fatherParam) {
    set(v);
}
//====================================================================
void OrientableCoord::set(const float x, const float y, const float z) {
  setX(x);
  setY(y);
  setZ(z);
}
//====================================================================
void OrientableCoord::set(const Coord& coord) {
  Coord::setX(coord.getX());
  Coord::setY(coord.getY());
  Coord::setZ(coord.getZ());
}
//====================================================================
void OrientableCoord::setX(float x) {		
  (this->*(father->writeX))(x);
}
//====================================================================
void OrientableCoord::setY(float y) {
  (this->*(father->writeY))(y);
}
//====================================================================
void OrientableCoord::setZ(float z) {
  (this->*(father->writeZ))(z);
}
//====================================================================
float OrientableCoord::getX() const {
  return (this->*(father->readX))();
}
//====================================================================
float OrientableCoord::getY() const {
  return (this->*(father->readY))();
}
//====================================================================
float OrientableCoord::getZ() const {
  return (this->*(father->readZ))();
}
//====================================================================
void OrientableCoord::setInvertedX(float x) {       
  Coord::setX(-x);
}
//====================================================================
void OrientableCoord::setInvertedY(float y) {
  Coord::setY(-y);
}
//====================================================================
void OrientableCoord::setInvertedZ(float z) {
  Coord::setZ(-z);
}
//====================================================================
float OrientableCoord::getInvertedX() const {
  return -Coord::getX();
}
//====================================================================
float OrientableCoord::getInvertedY() const {
  return -Coord::getY();
}
//====================================================================
float OrientableCoord::getInvertedZ() const {
  return -Coord::getZ();
}
//====================================================================
