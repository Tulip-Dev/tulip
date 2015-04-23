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

  GlSceneVisitor():threadSafe(false) {}
  virtual ~GlSceneVisitor() {}

  virtual void visit(GlSimpleEntity *) {}
  virtual void visit(GlNode *) {}
  virtual void visit(GlEdge *) {}
  virtual void visit(GlLayer *) {}
  virtual void reserveMemoryForNodes(unsigned int) {}
  virtual void reserveMemoryForEdges(unsigned int) {}
  bool isThreadSafe() {
    return threadSafe;
  }

protected :

  bool threadSafe;

};

}

#endif // Tulip_GLSCENEVISITOR_H
///@endcond
