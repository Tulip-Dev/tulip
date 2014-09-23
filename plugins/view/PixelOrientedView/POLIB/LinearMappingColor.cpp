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

#include "LinearMappingColor.h"


/*static void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v ) {
  int i;
  float f, p, q, t;

  if( s == 0 ) { // achromatic (grey)
    *r = *g = *b = v;
    return;
  }

  h /= 60;               // sector 0 to 5
  i = (int)floor( h );
  f = h - (float)i;      // factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );

  switch( i ) {
  case 0:
    *r = v;
    *g = t;
    *b = p;
    break;

  case 1:
    *r = q;
    *g = v;
    *b = p;
    break;

  case 2:
    *r = p;
    *g = v;
    *b = t;
    break;

  case 3:
    *r = p;
    *g = q;
    *b = v;
    break;

  case 4:
    *r = t;
    *g = p;
    *b = v;
    break;

  default:                // case 5:
    *r = v;
    *g = p;
    *b = q;
    break;
  }
  }*/

namespace pocore {
LinearMappingColor::LinearMappingColor(const double &min, const double &max) :
  _min(min),
  _max(max) {
  startColor[0] = 255;
  startColor[1] = 255;
  startColor[2] = 0;

  endColor[0] = 0;
  endColor[1] = 0;
  endColor[2] = 255;
}

RGBA LinearMappingColor::getColor(const double &value, unsigned int) const {
  double ratio = (value - _min) / (_max - _min);
  RGBA color;

  for (unsigned int i = 0; i<3 ; ++i) {
    color[i] = (unsigned char)(double(startColor[i]) + (double(endColor[i]) - double(startColor[i])) * ratio);
  }

  color[3] = 255;
  return color;
}

}
