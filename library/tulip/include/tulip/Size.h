//-*-c++-*-
#ifndef TULIP_SIZE_H
#define TULIP_SIZE_H
#include <tulip/Vector.h>
///

namespace tlp {

class TLP_SCOPE Size : public tlp::Vector<float,3> {
public:
  inline Size(const float width=0,const float height=0,const float depth=0);
  inline Size(const tlp::Vector<float,3> &);
  inline void set(const float width=0,const float height=0,const float depth=0);
  inline void set(const Size& size);
  inline void setW(const float width);
  inline void setH(const float height);
  inline void setD(const float depth);
  inline float getW() const;
  inline float getH() const;
  inline float getD() const;
  inline void get(float &width, float &height, float &depth) const;
};

Size::Size(const tlp::Vector<float,3> &v) : tlp::Vector<float,3>(v) {}
Size::Size(const float width,const float height,const float depth) {
  array[0]=width;
  array[1]=height;
  array[2]=depth;
}
void Size::set(const float width,const float height,const float depth) {
  array[0]=width;
  array[1]=height;
  array[2]=depth;
}
void Size::set(const Size &size) { (*this)=size; }
float Size::getW() const {return array[0];}
float Size::getH() const {return array[1];}
float Size::getD() const {return array[2];}
void Size::get(float &width,float &height,float &depth) const {
  width=array[0];
  height=array[1];
  depth=array[2];
}
void Size::setW(const float width) {array[0]=width;}
void Size::setH(const float height) {array[1]=height;}
void Size::setD(const float depth) {array[2]=depth;}

}
#endif
