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
#ifndef Tulip_GLCOMPLEXEENTITY_H
#define Tulip_GLCOMPLEXEENTITY_H

#include <vector>

#include "tulip/GlEntity.h"
#include "tulip/GlSceneVisitor.h"
#include "tulip/Camera.h"
#include "tulip/GlGraphInputData.h"

namespace tlp {

  class OcclusionTest;
  class TextRenderer;

  class TLP_GL_SCOPE GlComplexeEntity : public GlEntity {

  public:
    
    virtual BoundingBox getBoundingBox(GlGraphInputData* data) = 0;
    
    virtual void draw(float lod,GlGraphInputData* data,Camera* camera) = 0;

    virtual void drawLabel(bool drawSelect,bool drawNodesLabel,bool drawEdgesLabel,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data) = 0;
  };
  
}

#endif // Tulip_GLCOMPLEXEENTITY_H
