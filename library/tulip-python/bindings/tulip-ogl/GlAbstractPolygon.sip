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

class GlAbstractPolygon : tlp::GlSimpleEntity {
%TypeHeaderCode
#include <tulip/GlAbstractPolygon.h>
%End
  public:
    
    GlAbstractPolygon();
    
    virtual ~GlAbstractPolygon();

    enum PolygonMode {POLYGON = 0, QUAD_STRIP = 1};
    
    
    void draw(float lod, tlp::Camera *camera);
    
    PolygonMode getPolygonMode();
    
    void setPolygonMode(PolygonMode mode);

    bool getFillMode();

    void setFillMode(const bool);

    bool getOutlineMode();

    void setOutlineMode(const bool);

    bool getLightingMode();

    void setLightingMode(const bool);

    tlp::Color getFillColor(unsigned int i);

    void setFillColor(unsigned int i, const tlp::Color &color);
    
    void setFillColor(const tlp::Color &color);
    
    tlp::Color getOutlineColor(unsigned int i);

    void setOutlineColor(unsigned int i, const tlp::Color &color);

    void setOutlineColor(const tlp::Color &color);

    std::string getTextureName();

    void setTextureName(const std::string &name);

    float getOutlineSize();

    void setOutlineSize(float size);

    float getHideOutlineLod();

    void setHideOutlineLod(float lod);


  protected:


    virtual void setPoints(const std::vector<tlp::Coord> &points);

    virtual void setPoint(unsigned int index, const tlp::Coord &point);

    virtual void setFillColors(const std::vector<tlp::Color> &colors);

    virtual void setOutlineColors(const std::vector<tlp::Color> &colors);

    virtual void recomputeBoundingBox();

    virtual void clearGenerated();

};

};
