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

    GlSceneVisitor():threadSafe(false){}
    virtual ~GlSceneVisitor() {}

    virtual void visit(GlSimpleEntity *) {}
    virtual void visit(GlNode *) {}
    virtual void visit(GlEdge *) {}
    virtual void visit(GlLayer *) {}
    virtual void reserveMemoryForNodes(unsigned int) {}
    virtual void reserveMemoryForEdges(unsigned int) {}
    bool isThreadSafe(){return threadSafe;}

  protected :

    bool threadSafe;

  };
  
}

#endif // Tulip_GLSCENEVISITOR_H
