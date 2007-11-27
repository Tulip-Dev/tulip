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
#ifndef Tulip_GLLODSCENEVISITOR_H
#define Tulip_GLLODSCENEVISITOR_H


#include <vector>
#include <map>

#include <tulip/BoundingBox.h>

#include "tulip/GlSceneVisitor.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/GlLODCalculator.h"

namespace tlp {

  class TLP_GL_SCOPE GlLODSceneVisitor : public GlSceneVisitor{

  public:

    GlLODSceneVisitor(GlLODCalculator *calculator,GlGraphInputData* inputData)
      : calculator(calculator),inputData(inputData) {}

    virtual void visit(GlSimpleEntity *entity);
    virtual void visit(GlNode *glNode);
    virtual void visit(GlEdge *glEdge);
    virtual void visit(GlLayer *layer);

  private:
    
    GlLODCalculator* calculator;
    GlGraphInputData* inputData;

  };
  
}

#endif // Tulip_GLLODSCENEVISITOR_H
