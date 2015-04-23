/**
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
#include <cmath>
#include <tulip/Color.h>

static void RGBtoHSV(unsigned char r, unsigned char g, unsigned char b, int &h, int &s, int &v);
static void HSVtoRGB(int h, int s, int v, unsigned char &r, unsigned char &g, unsigned char &b);

const tlp::Color tlp::Color::Amaranth(229, 43, 80);
const tlp::Color tlp::Color::Amber(255, 191, 0);
const tlp::Color tlp::Color::Apricot(251, 206, 177);
const tlp::Color tlp::Color::Aquamarine(127, 255, 212);
const tlp::Color tlp::Color::Azure(0, 127, 255);
const tlp::Color tlp::Color::BabyBlue(137, 207, 240);
const tlp::Color tlp::Color::Beige(245, 245, 220);
const tlp::Color tlp::Color::Black(0,0,0);
const tlp::Color tlp::Color::Blue(0,0,255);
const tlp::Color tlp::Color::BlueGreen(0, 149, 182);
const tlp::Color tlp::Color::BlueViolet(138, 43, 226);
const tlp::Color tlp::Color::Blush(222, 93, 131);
const tlp::Color tlp::Color::Bronze(205, 127, 50);
const tlp::Color tlp::Color::Brown(150, 75, 0);
const tlp::Color tlp::Color::Burgundy(128, 0, 32);
const tlp::Color tlp::Color::Byzantium(112, 41, 99);
const tlp::Color tlp::Color::Carmine(150, 0, 24);
const tlp::Color tlp::Color::Cerise(222, 49, 99);
const tlp::Color tlp::Color::Cerulean(0, 123, 167);
const tlp::Color tlp::Color::Champagne(247, 231, 206);
const tlp::Color tlp::Color::ChartreuseGreen(127, 255, 0);
const tlp::Color tlp::Color::Chocolate(123, 63, 0);
const tlp::Color tlp::Color::Coffee(111, 78, 55);
const tlp::Color tlp::Color::Copper(184, 115, 51);
const tlp::Color tlp::Color::Coral(248, 131, 121);
const tlp::Color tlp::Color::Crimson(220, 20, 60);
const tlp::Color tlp::Color::Cyan(0, 255, 255);
const tlp::Color tlp::Color::DesertSand(237, 201, 175);
const tlp::Color tlp::Color::ElectricBlue(125, 249, 255);
const tlp::Color tlp::Color::Erin(0, 255, 63);
const tlp::Color tlp::Color::Gold(255, 215, 0);
const tlp::Color tlp::Color::Gray(128, 128, 128);
const tlp::Color tlp::Color::Green(0, 255, 0);
const tlp::Color tlp::Color::Harlequin(63, 255, 0);
const tlp::Color tlp::Color::Indigo(75, 0, 130);
const tlp::Color tlp::Color::Ivory(255, 255, 240);
const tlp::Color tlp::Color::Jade(0, 168, 107);
const tlp::Color tlp::Color::JungleGreen(41, 171, 135);
const tlp::Color tlp::Color::Lavender(181, 126, 220);
const tlp::Color tlp::Color::Lemon(255, 247, 0);
const tlp::Color tlp::Color::Lilac(200, 162, 200);
const tlp::Color tlp::Color::Lime(191, 255, 0);
const tlp::Color tlp::Color::Magenta(255, 0, 255);
const tlp::Color tlp::Color::MagentaRose(255, 0, 175);
const tlp::Color tlp::Color::Maroon(128, 0, 0);
const tlp::Color tlp::Color::Mauve(224, 176, 255);
const tlp::Color tlp::Color::NavyBlue(0, 0, 128);
const tlp::Color tlp::Color::Olive(128, 128, 0);
const tlp::Color tlp::Color::Orange(255, 165, 0);
const tlp::Color tlp::Color::OrangeRed(255, 69, 0);
const tlp::Color tlp::Color::Orchid(218, 112, 214);
const tlp::Color tlp::Color::Peach(255, 229, 180);
const tlp::Color tlp::Color::Pear(209, 226, 49);
const tlp::Color tlp::Color::Periwinkle(204, 204, 255);
const tlp::Color tlp::Color::PersianBlue(28, 57, 187);
const tlp::Color tlp::Color::Pink(255, 192, 203);
const tlp::Color tlp::Color::Plum(142, 69, 133);
const tlp::Color tlp::Color::PrussianBlue(0, 49, 83);
const tlp::Color tlp::Color::Puce(204, 136, 153);
const tlp::Color tlp::Color::Purple(128, 0, 128);
const tlp::Color tlp::Color::Raspberry(227, 11, 92);
const tlp::Color tlp::Color::Red(255, 0, 0);
const tlp::Color tlp::Color::RedViolet(199, 21, 133);
const tlp::Color tlp::Color::Rose(255, 0, 127);
const tlp::Color tlp::Color::Salmon(250, 128, 114);
const tlp::Color tlp::Color::Sapphire(15, 82, 186);
const tlp::Color tlp::Color::Scarlet(255, 36, 0);
const tlp::Color tlp::Color::Silver(192, 192, 192);
const tlp::Color tlp::Color::SlateGray(112, 128, 144);
const tlp::Color tlp::Color::SpringBud(167, 252, 0);
const tlp::Color tlp::Color::SpringGreen(0, 255, 127);
const tlp::Color tlp::Color::Tan(210, 180, 140);
const tlp::Color tlp::Color::Taupe(72, 60, 50);
const tlp::Color tlp::Color::Teal(0, 128, 128);
const tlp::Color tlp::Color::Turquoise(64, 224, 208);
const tlp::Color tlp::Color::Violet(238, 130, 238);
const tlp::Color tlp::Color::Viridian(64, 130, 109);
const tlp::Color tlp::Color::White(255, 255, 255);
const tlp::Color tlp::Color::Yellow(255, 255, 0);

//=================================================================
/*
  Redefined in order to output char in numerical format
*/
std::ostream& tlp::operator<<(std::ostream &os,const tlp::Color &a) {
  const unsigned int SIZE =4;
  os << "(" ;

  for (unsigned int i=0 ; i<SIZE ; ++i) {
    if( i>0 )
      os << ",";

    os << (unsigned int)a[i];
  }

  os << ")" ;
  return os;
}
//=================================================================
/*
  Redefined in order to input char in numerical format
*/
std::istream & tlp::operator>> (std::istream &is, tlp::Color & outA) {
  const unsigned int SIZE =4;
  char c;
  int pos = is.tellg();
  is.clear();

  if(!bool(is >> c) || c!='(') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  for(unsigned int i=0; i<SIZE; ++i) {
    if(i>0 && ( !bool(is >> c) || c!=',') ) {
      is.seekg(pos);
      is.setstate(std::ios::failbit);
      return is;
    }

    unsigned int vi = 0;
    bool done = bool(is >> vi);
    outA.array[i] = vi;

    if(!done) {
      is.seekg(pos);
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  if(!bool(is >> c) || c!=')') {
    is.seekg(pos);
    is.setstate(std::ios::failbit);
    return is;
  }

  return is;
}
//=================================================================



long tlp::Color::getTrueColor() {
  long ret=0;
  long tmp;
  unsigned int RR=array[0],BB=array[1],GG=array[2];
  tmp=RR << 16;
  ret=tmp;
  tmp=GG << 8;
  ret+=tmp;
  ret+=BB;
  return ret;
}

// HSV accessors
#define HSVGet(P)                               \
int tlp::Color::get##P() const {                     \
  int H,S,V;                                    \
  RGBtoHSV(array[0],array[1],array[2],H,S,V);   \
  return P;                                     \
}

HSVGet(H)
HSVGet(S)
HSVGet(V)
#undef HSVGet

#define HSVSet(P)                               \
void tlp::Color::set##P(int val) {                   \
  int H,S,V;                                    \
  RGBtoHSV(array[0],array[1],array[2],H,S,V);   \
  P = val;                                      \
  HSVtoRGB(H,S,V,array[0],array[1],array[2]);   \
}

HSVSet(H)
HSVSet(S)
HSVSet(V)
#undef HSVSet

//=================================================================
//// static RGB<->HSV conversion functions
void RGBtoHSV(unsigned char r, unsigned char g, unsigned char b, int &h, int &s, int &v) {
  int theMin, theMax, delta;
  theMin = std::min(std::min(r, g), b); //  r <? g <? b
  theMax = std::max(std::max(r, g), b); //  r >? g >? b
  v = theMax;       // v

  delta = theMax - theMin;

  if((theMax != 0) && (delta != 0))
    s = 255 * delta / theMax;   // s
  else {
    // r=g=b = 0    // s = 0, v is undefined
    s = 0;
    h = -1;
    return;
  }

  if(r == theMax)
    h = (int) (60 * (float)(g - b) / (float)delta);   // between yellow & magenta
  else if(g == theMax)
    h = (int) (60 * (2.0f + (float)(b - r) / (float)delta));  // between cyan & yellow
  else
    h = (int) (60 * (4.0f + (float)(r - g) / (float)delta));  // between magenta & cyan

  if(h < 0)
    h += 360;
}

void HSVtoRGB(int h, int s, int v, unsigned char &r, unsigned char &g, unsigned char &b) {
  int i;
  int p, q, t;
  float f, sf = s/255.0;

  if (v<0) v = 0;
  else if (v>255) v = 255;

  if (s<0) s = 0;
  else if (s>255) s = 255;

//   h %= 360;
//   if (h<0) h+=360;

  if(s == 0) {
    // achromatic (grey)
    r = g = b = v;
    return;
  }

  i = h/60;     // sector 0 to 5
  f = (h/60.0f - i);      // factorial part of h
  p = (int) (v * (1 - sf));
  q = (int) (v * (1 - sf * f));
  t = (int) (v * (1 - sf * (1 - f)));

  switch(i) {
  case 0:
    r = v;
    g = t;
    b = p;
    break;

  case 1:
    r = q;
    g = v;
    b = p;
    break;

  case 2:
    r = p;
    g = v;
    b = t;
    break;

  case 3:
    r = p;
    g = q;
    b = v;
    break;

  case 4:
    r = t;
    g = p;
    b = v;
    break;

  default:    // case 5:
    r = v;
    g = p;
    b = q;
    break;
  }
}
//=================================================================
