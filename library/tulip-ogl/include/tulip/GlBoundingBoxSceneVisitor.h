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
#ifndef Tulip_GLBOUNDINGBOXSCENEVISITOR_H
#define Tulip_GLBOUNDINGBOXSCENEVISITOR_H

#include <tulip/BoundingBox.h>

#include "tulip/GlSceneVisitor.h"

#include "tulip/GlGraphInputData.h"

namespace tlp {

  class TLP_GL_SCOPE GlBoundingBoxSceneVisitor : public GlSceneVisitor{
    
  public:
    
    GlBoundingBoxSceneVisitor(GlGraphInputData* inputData):bbIsInit(false),inputData(inputData){}
    
    virtual void visit(GlSimpleEntity *entity);
    virtual void visit(GlNode *glNode);
    virtual void visit(GlEdge *glEdge);
    
    BoundingBox getBoundingBox() {return boundingBox;}
    
  private:
    
    bool bbIsInit;
    BoundingBox boundingBox;
    GlGraphInputData* inputData;

  };
  
}

#endif // Tulip_GLLODSCENEVISITOR_H
