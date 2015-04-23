/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLSELECTSCENEVISITOR_H
#define Tulip_GLSELECTSCENEVISITOR_H

#include <tulip/GlSceneVisitor.h>

namespace tlp {

enum SelectionFlag {
  SelectSimpleEntities=1,
  SelectNodes=2,
  SelectEdges=4
};

class GlGraphInputData;
class GlLODCalculator;

/**
 * This visitor is use when we want to select an entity in scene
 */
class TLP_GL_SCOPE GlSelectSceneVisitor : public GlSceneVisitor {

public:

  /**
   * Constructor with SelectionFlac (SelectSimpleEntity, SelectNodes and SelectEdges), GlGraphInputData and GlLODCalculator
   */
  GlSelectSceneVisitor(SelectionFlag flag,GlGraphInputData* inputData,GlLODCalculator *calculator);

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
///@endcond
