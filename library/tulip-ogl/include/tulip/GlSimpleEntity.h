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

#include "tulip/GlEntity.h"
#include "tulip/GlSceneVisitor.h"

namespace tlp {

  class TLP_GL_SCOPE GlSimpleEntity : public GlEntity {

  public:

    GlSimpleEntity():visible(true) {}

    virtual void draw(float lod) = 0;
    
    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      visitor->visit(this);
    }

    virtual void setVisible(bool visible) {this->visible=visible;}
    virtual bool isVisible() {return visible;}

    virtual BoundingBox getBoundingBox() {return boundingBox;}

  protected:
    
    bool visible;
    
    BoundingBox boundingBox;

  };
  
}

#endif // Tulip_GLSIMPLEENTITY_H
