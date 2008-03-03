//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 16/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLMETANODE_H
#define Tulip_GLMETANODE_H

#include "tulip/GlNode.h"

namespace tlp {
  
  class TLP_GL_SCOPE GlMetaNode : public GlNode{
  
  public:

    GlMetaNode(unsigned int id):GlNode(id) {}

    virtual void draw(float lod,GlGraphInputData *data,Camera* camera);

    virtual void drawLabel(bool drawSelect,bool drawNodesLabel,bool drawEdgesLabel,OcclusionTest* test,TextRenderer* renderer,GlGraphInputData* data);
    
  };
  
}

#endif // Tulip_GLMETANODE_H
