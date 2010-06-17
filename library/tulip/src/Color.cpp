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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include "tulip/Color.h"

static void RGBtoHSV(unsigned char r, unsigned char g, unsigned char b, int &h, int &s, int &v);
static void HSVtoRGB(int h, int s, int v, unsigned char &r, unsigned char &g, unsigned char &b);


//=================================================================
/*
  Redefined in order to output char in numerical format
*/
std::ostream& tlp::operator<<(std::ostream &os,const tlp::Color &a) {
  const unsigned int SIZE =4;
  os << "(" ;
  for ( unsigned int i=0 ; i<SIZE ; ++i ) {
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
  if( !(is >> c) || c!='(' ) {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  for( unsigned int i=0;i<SIZE;++i ) {
    if( i>0 && ( !(is >> c) || c!=',' ) ) {
      is.seekg(pos);
      is.setstate( std::ios::failbit );
      return is;  
    }
    bool done = true;
    unsigned int vi;
    done = ( is >> vi );
    outA.array[i] = vi;
    if( !done ) {
      is.seekg(pos);
      is.setstate( std::ios::failbit );
      return is;
    }
  }
  if( !(is >> c) || c!=')' ) {
    is.seekg(pos);
    is.setstate( std::ios::failbit );
    return is;  
  }
  return is;  
}
//=================================================================

long tlp::Color::getTrueColor()
{
  long ret=0;long tmp;
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

HSVGet(H);
HSVGet(S);
HSVGet(V);
#undef HSVGet

#define HSVSet(P)                               \
void tlp::Color::set##P(int val) {                   \
  int H,S,V;                                    \
  RGBtoHSV(array[0],array[1],array[2],H,S,V);   \
  P = val;                                      \
  HSVtoRGB(H,S,V,array[0],array[1],array[2]);   \
}

HSVSet(H);
HSVSet(S);
HSVSet(V);
#undef HSVSet

//=================================================================
//// static RGB<->HSV conversion functions
void RGBtoHSV(unsigned char r, unsigned char g, unsigned char b, int &h, int &s, int &v) {
  int theMin, theMax, delta;
  theMin = std::min(std::min(r, g), b); //  r <? g <? b
  theMax = std::max(std::max(r, g), b); //  r >? g >? b
  v = theMax;				// v

  delta = theMax - theMin;

  if((theMax != 0) && (delta != 0))
    s = 255 * delta / theMax;		// s
  else {
    // r=g=b = 0		// s = 0, v is undefined
    s = 0;
    h = -1;
    return;
  }

  if(r == theMax)
    h = (int) (60 * (float)(g - b) / (float)delta);		// between yellow & magenta
  else if(g == theMax)
    h = (int) (60 * (2.0f + (float)(b - r) / (float)delta));	// between cyan & yellow
  else
    h = (int) (60 * (4.0f + (float)(r - g) / (float)delta));	// between magenta & cyan

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

  i = h/60;			// sector 0 to 5
  f = (h/60.0f - i);			// factorial part of h
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
  default:		// case 5:
    r = v;
    g = p;
    b = q;
    break;
  }
}
//=================================================================
