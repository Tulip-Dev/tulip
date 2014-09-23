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

#ifndef Tulip_GLLODSCENEVISITOR_H
#define Tulip_GLLODSCENEVISITOR_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <tulip/GlSceneVisitor.h>

namespace tlp {

class GlGraphInputData;
class GlLODCalculator;

/**
 * Visitor to colect the Bounding box of entities in the scene
 */
class TLP_GL_SCOPE GlLODSceneVisitor : public GlSceneVisitor {

public:

  /**
   * Basic constructor
   */
  GlLODSceneVisitor(GlLODCalculator *calculator,const GlGraphInputData* inputData)
    : calculator(calculator),inputData(inputData) {
    threadSafe=true;
  }

  /**
   * Visit a GlSimpleEntity
   */
  virtual void visit(GlSimpleEntity *entity);
  /**
   * Visit a node
   */
  virtual void visit(GlNode *glNode);
  /**
   * Visit an Edge
   */
  virtual void visit(GlEdge *glEdge);
  /**
   * Visit a layer
   */
  virtual void visit(GlLayer *layer);

  /**
   * Reserve memory to store nodes LOD
   */
  virtual void reserveMemoryForNodes(unsigned int numberOfNodes);

  /**
   * Reserve memory to store edges LOD
   */
  virtual void reserveMemoryForEdges(unsigned int numberOfEdges);

private:

  GlLODCalculator* calculator;
  const GlGraphInputData* inputData;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODSCENEVISITOR_H
///@endcond
