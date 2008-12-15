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

  /**
   * This visitor is use when we want to select an entity in scene
   */
  class TLP_GL_SCOPE GlSelectSceneVisitor : public GlSceneVisitor{

  public:

    /**
     * Constructor with SelectionFlac (SelectSimpleEntity, SelectNodes and SelectEdges), GlGraphInputData and GlLODCalculator
     */
    GlSelectSceneVisitor(SelectionFlag flag,GlGraphInputData* inputData,GlLODCalculator *calculator)
      :selectionFlag(flag),inputData(inputData),calculator(calculator) {}

    /**
     * Visit a simple entity
     */
    virtual void visit(GlSimpleEntity *entity);
    /**
     * Visit a node
     */
    virtual void visit(GlNode *glNode);
    /**
     * Visit an edge
     */
    virtual void visit(GlEdge *glEdge);
    /**
     * Visit a layer
     */
    virtual void visit(GlLayer *layer);

  private:

    SelectionFlag selectionFlag;

    GlGraphInputData* inputData;

    GlLODCalculator* calculator;

  };

}

#endif // Tulip_GLSELECTSCENEVISITOR_H
