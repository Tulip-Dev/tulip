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

class GlPolygon : tlp::GlAbstractPolygon {
%TypeHeaderCode
#include <tulip/GlPolygon.h>
%End
  public:
    
    GlPolygon(const bool filled = true,
	      const bool outlined= true,
        const std::string &textureName="",
        const float outlineSize = 1);
    
    GlPolygon(const std::vector<tlp::Coord> &points,
	      const std::vector<tlp::Color> &fillColors,
	      const std::vector<tlp::Color> &outlineColors,
	      const bool filled,
	      const bool outlined,
        const std::string &textureName="",
        const float outlineSize = 1);
    
    GlPolygon(const unsigned int nbPoints,
	      const unsigned int nbFillColors,
	      const unsigned int nbOutlineColors,
	      const bool filled = true,
	      const bool outlined = true,
        const std::string &textureName="",
        const float outlineSize = 1);
        
    virtual ~GlPolygon();

    virtual void resizePoints(const unsigned int nbPoints);

    virtual void resizeColors(const unsigned int nbColors);

    
  };

};

