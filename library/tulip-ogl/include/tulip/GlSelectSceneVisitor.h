//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 25/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLSELECTSCENEVISITOR_H
#define Tulip_GLSELECTSCENEVISITOR_H

#include <vector>
#include <utility>

#include "tulip/GlSceneVisitor.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlLODCalculator.h"

namespace tlp {

  enum SelectionFlag {
    SelectSimpleEntities=1,
    SelectNodes=2,
    SelectEdges=4
  };

  class TLP_GL_SCOPE GlSelectSceneVisitor : public GlSceneVisitor{

  public:

    GlSelectSceneVisitor(SelectionFlag flag,GlGraphInputData* inputData,GlLODCalculator *calculator)
      :selectionFlag(flag),inputData(inputData),calculator(calculator) {}

    virtual void visit(GlSimpleEntity *entity);
    virtual void visit(GlNode *glNode);
    virtual void visit(GlEdge *glEdge);
    virtual void visit(GlLayer *layer);

  private:

    SelectionFlag selectionFlag;

    GlGraphInputData* inputData;

    GlLODCalculator* calculator;

  };
  
}

#endif // Tulip_GLSELECTSCENEVISITOR_H
