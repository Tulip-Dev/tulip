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

// This implementation of HSI Color Space is from the following paper : "Issues in Visualizing Large Databases"
// D. A. Keim, H.-P. Kriegel

#include "HSIColorSpace.h"

#include <iostream>
#include <cmath>

using namespace std;

namespace pocore {

/***************************** Constructor **********************************/
HSI::HSI(double h, double s, double i): hue( h<6 ? h : h-6 ), saturation(s), intensity(i) {}

/************************** Conversion from RGB to HSI ***********************/
HSI::HSI(const RGBA& c) {
  double red = c[0], green = c[1], blue = c[2];
  double mid = (red + green + blue) / 3.0;
  double mr = red - mid;
  double mg = green - mid;
  double mb = blue - mid;
  double cos_hue = (2.0 * mr - mg - mb) / sqrt((mr*mr + mg*mg + mb*mb) * 6.0);
  intensity = mid + sqrt (2.0 * (mr*mr + mg*mg + mb*mb) / 3.0);
  saturation = 2.0 * (intensity - mid) / intensity;
  hue = acos( cos_hue ) * 3.0 / M_PI;

  if (blue > green)
    hue = 6.0 - hue;
}

/************************** Conversion from HSI to RGB ***********************/
RGBA HSI::getRGBA() const {
  RGBA ret;

  if (saturation == 0.0) {
    ret[0] = (unsigned char) (intensity * 255);
    ret[1] = (unsigned char) (intensity * 255);
    ret[2] = (unsigned char) (intensity * 255);
    ret[3] = 255;
  }
  else {
    ret[0] = (unsigned char) (value(hue + 0.0) * 255);                    // 0 Grad = 0 pi
    ret[1] = (unsigned char) (value(hue + 4.0) * 255);                  // 240 Grad = 4/3 pi
    ret[2] = (unsigned char) (value(hue + 2.0) * 255);                   // 120 Grad = 2/3 pi
    ret[3] = 255;
  }

  return ret;
}

double HSI::value(double hue_phase) const {
  double pure = 0.5 * (1 + cos(hue_phase * M_PI / 3.0));
  return (intensity * (1.0 - saturation * (1.0 - pure)));
}

/************************* Generation of HSIColorScales ************************/
HSIColorScale :: HSIColorScale( const HSI& from, const HSI& to )
  : foot(from)
  , hue_range(to.hue - from.hue)
  , sat_range(to.saturation - from.saturation)
  , int_range(to.intensity - from.intensity) {
  if( hue_range < 0.0 )
    hue_range += 6.0;
}

HSI HSIColorScale::operator[](double f) const {
  return HSI(hue_range * f + foot.hue,
             sat_range * f + foot.saturation,
             int_range * f + foot.intensity);
}

}
