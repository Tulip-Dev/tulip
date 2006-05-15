//-*-c++-*-
#ifndef TULIP_COORD_H
#define TULIP_COORD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <tulip/tulipconf.h>
#include <tulip/Vector.h>
#include <cassert>
#include <ostream>

/// Class for coordinates in 3D
namespace tlp {

class TLP_SCOPE Coord : public tlp::Vector<float,3> {
public:
  inline Coord(const float=0,const float=0,const float=0);
  inline Coord(const tlp::Vector<float,3> &);
  inline void set(const float=0,const float=0,const float=0);
  inline void set(const Coord&);
  inline void setX(float);
  inline void setY(float);
  inline  void setZ(float);
  float getX() const {return (*this)[0];}
  float getY() const {return (*this)[1];}
  float getZ() const {return (*this)[2];}
  inline void get(float &, float &, float &) const;

  inline Coord operator+(const  tlp::Vector<float,3> &) const;
  inline Coord operator+(const float &) const;
  inline Coord operator-(const  tlp::Vector<float,3> &) const;
  inline Coord operator-(const float &) const;
  inline Coord operator*(const  tlp::Vector<float,3> &) const;
  inline Coord operator*(const float &) const ;
  inline Coord operator/(const  tlp::Vector<float,3> &) const;
  inline Coord operator/(const float &) const;
  inline Coord operator^(const  tlp::Vector<float,3> &) const;
};
}

tlp::Coord tlp::Coord::operator+(const  tlp::Vector<float,3> &v) const {
  return tlp::Coord(*this)+=v;
}
//======================================================
tlp::Coord tlp::Coord::operator+(const float& scalaire) const {
  return tlp::Coord(*this)+=scalaire;
}
//======================================================
tlp::Coord tlp::Coord::operator-(const  tlp::Vector<float,3> &v) const { 
  return tlp::Coord(*this)-=v;
}
//======================================================
tlp::Coord tlp::Coord::operator-(const float& scalaire) const {
  return tlp::Coord(*this)-=scalaire;
}
//======================================================
tlp::Coord tlp::Coord::operator*(const  tlp::Vector<float,3> &v) const { 
  return tlp::Coord(*this)*=v;
}
//======================================================
tlp::Coord  tlp::Coord::operator*(const float& scalaire) const {
  return tlp::Coord(*this)*=scalaire;
}
//======================================================
tlp::Coord tlp::Coord::operator/(const  tlp::Vector<float,3> &v) const { 
  return tlp::Coord(*this)/=v;
}
//======================================================
tlp::Coord tlp::Coord::operator/(const float& scalaire) const {
  return tlp::Coord(*this)/=scalaire;
}
//======================================================
tlp::Coord tlp::Coord::operator^(const  tlp::Vector<float,3> &v) const {
  return tlp::Coord(*this) ^= v;
}

tlp::Coord::Coord(const float xx, const float yy, const float zz) {
  (*this)[0]=xx;(*this)[1]=yy,(*this)[2]=zz;
}
tlp::Coord::Coord(const tlp::Vector<float,3> &v) : tlp::Vector<float,3>(v) {}
void tlp::Coord::set(const float xx,const float yy,const float zz){
  (*this)[0]=xx;(*this)[1]=yy,(*this)[2]=zz;
}
void tlp::Coord::set(const tlp::Coord &c){
  for (int i=0;i<3;++i) (*this)[i]=c[i];
}
void tlp::Coord::get(float &xx,float &yy,float &zz) const {
  xx=(*this)[0];yy=(*this)[1];zz=(*this)[2];
}
void tlp::Coord::setX(const float xx) {
  (*this)[0]=xx;
}
void tlp::Coord::setY(const float yy) {
  (*this)[1]=yy;
}
void tlp::Coord::setZ(const float zz) {
  (*this)[2]=zz;
}
#endif
