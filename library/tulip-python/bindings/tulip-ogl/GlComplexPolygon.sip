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

%ModuleHeaderCode
typedef std::vector<tlp::Coord> std_vector_0100tlp_Coord;
%End

typedef std::vector<tlp::Coord> std_vector_0100tlp_Coord;

namespace tlp {

class GlComplexPolygon : tlp::GlSimpleEntity {
%TypeHeaderCode
#include <tulip/GlComplexPolygon.h>
%End  
	
  public:
    
    GlComplexPolygon();
    
    GlComplexPolygon(const std::vector<std_vector_0100tlp_Coord>&coords, tlp::Color fcolor, int polygonEdgesType=0, const std::string &textureName = "");

    GlComplexPolygon(const std::vector<std_vector_0100tlp_Coord>&coords, tlp::Color fcolor, tlp::Color ocolor,int polygonEdgesType=0,const std::string &textureName = "");
    
    GlComplexPolygon(const std::vector<tlp::Coord> &coords, tlp::Color fcolor, int polygonEdgesType=0, const std::string &textureName = "");

    GlComplexPolygon(const std::vector<tlp::Coord> &coords, tlp::Color fcolor, tlp::Color ocolor, int polygonEdgesType=0, const std::string &textureName = "");

	virtual ~GlComplexPolygon();

    virtual void draw(float lod, tlp::Camera *camera);

    void setOutlineMode(const bool);

    void setOutlineSize(double size);

    tlp::Color getFillColor();

    void setFillColor(const tlp::Color &color);

    tlp::Color getOutlineColor();

    void setOutlineColor(const tlp::Color &color);

    float getTextureZoom();

    void setTextureZoom(float zoom);

  };

};

