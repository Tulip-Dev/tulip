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
#ifndef DOXYGEN_NOTFOR_DEVEL


#include <vector>
#include <map>

#include <tulip/BoundingBox.h>

#include "tulip/GlSceneVisitor.h"

namespace tlp {

  class GlGraphInputData;
  class GlLODCalculator;

  /**
   * Visitor to colect the Bounding box of entities in the scene
   */
  class TLP_GL_SCOPE GlLODSceneVisitor : public GlSceneVisitor{

  public:

    /**
     * Basic constructor 
     */
    GlLODSceneVisitor(GlLODCalculator *calculator,GlGraphInputData* inputData)
      : calculator(calculator),inputData(inputData) {}

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
    GlGraphInputData* inputData;

  };
  
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLLODSCENEVISITOR_H
