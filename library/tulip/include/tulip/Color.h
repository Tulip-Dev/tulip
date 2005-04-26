//-*-c++-*-
#ifndef _COLOR_H
#define _COLOR_H
#include <tulip/Vector.h>
///
class Color : public tlp::Vector<unsigned char,4>{
 public:
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

std::ostream& operator<<(std::ostream &os,const Color &);
std::istream& operator>>(std::istream &is, Color &);

Color::Color(const unsigned char red ,const unsigned char green ,const unsigned char blue,const unsigned char alpha) {
  array[0]=red;
  array[1]=green;
  array[2]=blue;
  array[3]=alpha;
}
void Color::set(unsigned char red,unsigned char green,unsigned char blue, unsigned char alpha) {
  array[0]=red;
  array[1]=green;
  array[2]=blue;
  array[3]=alpha;
}

unsigned char Color::getR()const{return array[0];}
unsigned char Color::getG()const{return array[1];}
unsigned char Color::getB()const{return array[2];}
unsigned char Color::getA()const{return array[3];}

float Color::getRGL()const{return (float)array[0]/255.0;}
float Color::getGGL()const{return (float)array[1]/255.0;}
float Color::getBGL()const{return (float)array[2]/255.0;}
float Color::getAGL()const{return (float)array[3]/255.0;}
float* Color::getGL()const{
  float *result=new float[4];
  result[0]=getRGL();
  result[1]=getGGL();
  result[2]=getBGL();
  result[3]=getAGL();
  return result;
}

void Color::setR(unsigned char red){array[0]=red;}
void Color::setG(unsigned char green){array[1]=green;}
void Color::setB(unsigned char blue){array[2]=blue;}
void Color::setA(unsigned char alpha){array[3]=alpha;}

#endif

