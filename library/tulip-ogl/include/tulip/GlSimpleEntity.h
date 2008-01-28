//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLSIMPLEENTITY_H
#define Tulip_GLSIMPLEENTITY_H

#include <vector>

#include "tulip/GlEntity.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/Camera.h"

namespace tlp {

  class TLP_GL_SCOPE GlSimpleEntity : public GlEntity {

  public:

    GlSimpleEntity():visible(true),stencil(0xFFFF) {}

    virtual void draw(float lod,Camera* camera) = 0;
    
    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      visitor->visit(this);
    }

    void setVisible(bool visible) {this->visible=visible;}
    bool isVisible() {return visible;}
    virtual void setStencil(int stencil) {this->stencil=stencil;}
    int getStencil() {return stencil;}

    virtual BoundingBox getBoundingBox() {return boundingBox;}

    void addParent(GlLayer *layer) {parents.push_back(layer);}

    virtual void getXML(xmlNodePtr rootNode) =0;
    
    virtual void setWithXML(xmlNodePtr rootNode) =0;

  protected:
    
    bool visible;
    int stencil;

    BoundingBox boundingBox;

    std::vector<GlLayer*> parents;

  };
  
}

#endif // Tulip_GLSIMPLEENTITY_H
