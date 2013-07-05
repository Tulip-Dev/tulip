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
  
class GlRegularPolygon : tlp::GlPolygon {
%TypeHeaderCode
#include <tulip/GlRegularPolygon.h>
%End
  
  public:
  
    GlRegularPolygon(const tlp::Coord &position,
                     const tlp::Size &size,
                     unsigned int numberOfSides,
                     const tlp::Color &outlineColor = tlp::Color(255, 0 , 0 , 255),
                     const tlp::Color &fillColor = tlp::Color (0, 0, 255,255),
                     bool filled = true,
                     bool outlined =true,
                     const std::string &textureName="",
                     float outlineSize=1.);
  
    virtual ~GlRegularPolygon();

    void setStartAngle(float angle);

    unsigned int getNumberOfSides();

    void resizePoints(const unsigned int number);

    void setNumberOfSides(unsigned int number);

  protected :

    void computePolygon();


  };

};

