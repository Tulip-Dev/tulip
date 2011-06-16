/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {
 
class ColorScale {
%TypeHeaderCode
#include <tulip/ColorScale.h>
%End

public:

  ColorScale(const bool gradient = true);
  ColorScale(const std::vector<tlp::Color> &colors, const bool gradient = true);
  ColorScale(const tlp::ColorScale& scale);
  virtual ~ColorScale();

  virtual void setColorScale(const std::vector<tlp::Color> colors, const bool gradient = true);

  virtual void setColorAtPos(const float pos, const tlp::Color &color);
  	
  virtual tlp::Color getColorAtPos(const float pos) const;
	
  bool isGradient() const;
  
  std::map<float, tlp::Color> getColorMap() const;
  
};
};