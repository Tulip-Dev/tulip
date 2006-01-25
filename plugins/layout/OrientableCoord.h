#ifndef ORIENTABLECOORD_H
#define ORIENTABLECOORD_H

#include <tulip/Coord.h>
class OrientableLayout;

class OrientableCoord: public Coord {
 public: 
  OrientableCoord(OrientableLayout* fatherParam, const float x = 0,
		  const float y = 0, const float z = 0);
  OrientableCoord(OrientableLayout* fatherParam, const Coord& v);
  
  void  set(const float x = 0, const float y = 0, const float z = 0);  
  void  set(const Coord& v);
  
  void  setX(float x);
  void  setY(float y);
  void  setZ(float z);
  
  float getX() const;
  float getY() const;
  float getZ() const;
  
  void  setInvertedX(float x);
  void  setInvertedY(float y);
  void  setInvertedZ(float z);
  
  float getInvertedX() const;
  float getInvertedY() const;
  float getInvertedZ() const;
  
 protected:	
  OrientableLayout* father;
};

#endif
