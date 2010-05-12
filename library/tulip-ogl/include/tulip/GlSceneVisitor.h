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
#ifndef Tulip_GLSCENEVISITOR_H
#define Tulip_GLSCENEVISITOR_H

#include <tulip/tulipconf.h>

namespace tlp {

  class GlSimpleEntity;
  class GlNode;
  class GlEdge;
  class GlLayer;

  class TLP_GL_SCOPE GlSceneVisitor {

  public:

    virtual ~GlSceneVisitor() {}

    virtual void visit(GlSimpleEntity *entity) {}
    virtual void visit(GlNode *glNode) {}
    virtual void visit(GlEdge *glEdge) {}
    virtual void visit(GlLayer *layer) {}
    virtual void reserveMemoryForNodes(unsigned int numberOfNodes) {}
    virtual void reserveMemoryForEdges(unsigned int numberOfEdges) {}

  };
  
}

#endif // Tulip_GLSCENEVISITOR_H
