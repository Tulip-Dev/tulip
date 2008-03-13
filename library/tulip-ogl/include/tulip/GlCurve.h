//-*-c++-*-
/*
 Author: Auber David and Mathiaut Morgan
 Email : david.auber@tulip-software.org
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef Tulip_GLCURVE_H
#define Tulip_GLCURVE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>

#include <tulip/GlSimpleEntity.h>

namespace tlp {
  class TLP_GL_SCOPE GlCurve : public GlSimpleEntity {
  public:
    GlCurve(const std::vector<tlp::Coord> &points, 
	    const Color &beginFColor,
	    const Color &endFColor,
	    const float &beginSize=0.,
	    const float &endSize=0.);
    
    GlCurve(const unsigned int nbPoints = 3u);
    virtual ~GlCurve();

    void draw(float lod,Camera *camera);
    
    void setTexture(const std::string &texture);

    virtual void resizePoints(const unsigned int nbPoints);
    
    virtual const tlp::Coord& point(const unsigned int i) const;
    virtual tlp::Coord& point(const unsigned int i);

    /**
     * Translate entity
     */
    virtual void translate(const Coord& mouvement);

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);
    
    /**
     * Function to set data with XML
     */
    virtual void setWithXML(xmlNodePtr rootNode);

  protected:
    std::vector<tlp::Coord> _points;
    Color _beginFillColor;
    Color _endFillColor;
    float _beginSize;
    float _endSize;
    std::string texture;
  };
}
#endif
