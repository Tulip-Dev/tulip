/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

#ifndef _COLOR_H
#define _COLOR_H
#include <tulip/Vector.h>

///
namespace tlp {

class TLP_SCOPE Color : public tlp::Vector<unsigned char,4> {
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

  static const Color Amaranth;
  static const Color Amber;
  static const Color Apricot;
  static const Color Aquamarine;
  static const Color Azure;
  static const Color BabyBlue;
  static const Color Beige;
  static const Color Black;
  static const Color Blue;
  static const Color BlueGreen;
  static const Color BlueViolet;
  static const Color Blush;
  static const Color Bronze;
  static const Color Brown;
  static const Color Burgundy;
  static const Color Byzantium;
  static const Color Carmine;
  static const Color Cerise;
  static const Color Cerulean;
  static const Color Champagne;
  static const Color ChartreuseGreen;
  static const Color Chocolate;
  static const Color Coffee;
  static const Color Copper;
  static const Color Coral;
  static const Color Crimson;
  static const Color Cyan;
  static const Color DesertSand;
  static const Color ElectricBlue;
  static const Color Erin;
  static const Color Gold;
  static const Color Gray;
  static const Color Green;
  static const Color Harlequin;
  static const Color Indigo;
  static const Color Ivory;
  static const Color Jade;
  static const Color JungleGreen;
  static const Color Lavender;
  static const Color Lemon;
  static const Color Lilac;
  static const Color Lime;
  static const Color Magenta;
  static const Color MagentaRose;
  static const Color Maroon;
  static const Color Mauve;
  static const Color NavyBlue;
  static const Color Olive;
  static const Color Orange;
  static const Color OrangeRed;
  static const Color Orchid;
  static const Color Peach;
  static const Color Pear;
  static const Color Periwinkle;
  static const Color PersianBlue;
  static const Color Pink;
  static const Color Plum;
  static const Color PrussianBlue;
  static const Color Puce;
  static const Color Purple;
  static const Color Raspberry;
  static const Color Red;
  static const Color RedViolet;
  static const Color Rose;
  static const Color Salmon;
  static const Color Sapphire;
  static const Color Scarlet;
  static const Color Silver;
  static const Color SlateGray;
  static const Color SpringBud;
  static const Color SpringGreen;
  static const Color Tan;
  static const Color Taupe;
  static const Color Teal;
  static const Color Turquoise;
  static const Color Violet;
  static const Color Viridian;
  static const Color White;
  static const Color Yellow;

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

unsigned char tlp::Color::getR()const {
  return array[0];
}
unsigned char tlp::Color::getG()const {
  return array[1];
}
unsigned char tlp::Color::getB()const {
  return array[2];
}
unsigned char tlp::Color::getA()const {
  return array[3];
}

float tlp::Color::getRGL()const {
  return (float)array[0]/255.0;
}
float tlp::Color::getGGL()const {
  return (float)array[1]/255.0;
}
float tlp::Color::getBGL()const {
  return (float)array[2]/255.0;
}
float tlp::Color::getAGL()const {
  return (float)array[3]/255.0;
}
float* tlp::Color::getGL()const {
  float *result=new float[4];
  result[0]=getRGL();
  result[1]=getGGL();
  result[2]=getBGL();
  result[3]=getAGL();
  return result;
}

void tlp::Color::setR(unsigned char red) {
  array[0]=red;
}
void tlp::Color::setG(unsigned char green) {
  array[1]=green;
}
void tlp::Color::setB(unsigned char blue) {
  array[2]=blue;
}
void tlp::Color::setA(unsigned char alpha) {
  array[3]=alpha;
}

TLP_BEGIN_HASH_NAMESPACE {
  template <>
  struct hash<tlp::Color> {
    inline std::size_t operator()(const tlp::Color &c) const {
      return hash_vector(c);
    }
  };
} TLP_END_HASH_NAMESPACE

#endif

///@endcond
