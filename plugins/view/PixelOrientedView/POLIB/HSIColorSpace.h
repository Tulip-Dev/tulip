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


#ifndef HSI_H_
#define HSI_H_

#include "potypes.h"

namespace pocore {

class HSI {

public :

  HSI(double hue, double saturation, double intensity);
  HSI(const RGBA &c);
  double value(double hue_phase) const;
  RGBA getRGBA() const;

  double hue, saturation, intensity;

};

class HSIColorScale {

public :

  HSIColorScale(const HSI& from, const HSI& to);
  HSI operator[](double f) const;

private :

  HSI foot;
  double hue_range, sat_range, int_range;

};

}
#endif /* HSI_H_ */
