/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef _COLOR_H
#define _COLOR_H
#include <tulip/Vector.h>
///
namespace tlp {

class TLP_SCOPE Color : public tlp::Vector<unsigned char,4>{
 public:
  ///
  inline Color(const tlp::Vector<unsigned char,4>&);
  ///
  inline Color(const unsigned char red=0 ,const unsigned char green=0 ,const unsigned char blue=0,const unsigned char alpha=255);
  ///
  inline void set(const unsigned char red=0,const unsigned char green=0 ,const unsigned char blue=0, const unsigned char alpha=255);
  ///
  inline float getRGL()const;
  ///
  inline float getGGL()const;
  ///
  inline float getBGL()const;
  ///
  inline float getAGL()const;
  ///
  inline float* getGL()const;
  ///
  inline unsigned char getR()const;
  ///
  inline unsigned char getG()const;
  ///
  inline unsigned char getB()const;
  ///
  inline unsigned char getA()const;
  ///
  inline void setR(const unsigned char red);
  ///
  inline void setG(const unsigned char green);
  ///
  inline void setB(const unsigned char blue);
  ///
  inline void setA(const unsigned char alpha);
  ///
  long getTrueColor();
  ///
  int getH() const;
  ///
  int getS() const;
  ///
  int getV() const;
  ///
  void setH(int );
  ///
  void setS(int );
  ///
  void setV(int );
};

  TLP_SCOPE std::ostream& operator<<(std::ostream &os,const tlp::Color &);
  TLP_SCOPE std::istream& operator>>(std::istream &is, tlp::Color &);

}

tlp::Color::Color(const tlp::Vector<unsigned char,4> &v) : tlp::Vector<unsigned char,4>(v) {}

tlp::Color::Color(const unsigned char red ,const unsigned char green ,const unsigned char blue,const unsigned char alpha) {
  array[0]=red;
  array[1]=green;
  array[2]=blue;
  array[3]=alpha;
}
void tlp::Color::set(unsigned char red,unsigned char green,unsigned char blue, unsigned char alpha) {
  array[0]=red;
  array[1]=green;
  array[2]=blue;
  array[3]=alpha;
}

unsigned char tlp::Color::getR()const{return array[0];}
unsigned char tlp::Color::getG()const{return array[1];}
unsigned char tlp::Color::getB()const{return array[2];}
unsigned char tlp::Color::getA()const{return array[3];}

float tlp::Color::getRGL()const{return (float)array[0]/255.0;}
float tlp::Color::getGGL()const{return (float)array[1]/255.0;}
float tlp::Color::getBGL()const{return (float)array[2]/255.0;}
float tlp::Color::getAGL()const{return (float)array[3]/255.0;}
float* tlp::Color::getGL()const{
  float *result=new float[4];
  result[0]=getRGL();
  result[1]=getGGL();
  result[2]=getBGL();
  result[3]=getAGL();
  return result;
}

void tlp::Color::setR(unsigned char red){array[0]=red;}
void tlp::Color::setG(unsigned char green){array[1]=green;}
void tlp::Color::setB(unsigned char blue){array[2]=blue;}
void tlp::Color::setA(unsigned char alpha){array[3]=alpha;}
#endif

