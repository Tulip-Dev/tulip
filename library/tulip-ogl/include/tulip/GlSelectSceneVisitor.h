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
