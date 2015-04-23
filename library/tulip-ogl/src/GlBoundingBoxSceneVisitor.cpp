/**
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
#include <tulip/GlBoundingBoxSceneVisitor.h>

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>

using namespace std;

namespace tlp {

void GlBoundingBoxSceneVisitor::visit(GlSimpleEntity *entity) {
  if(entity->isVisible()) {
    BoundingBox bb=entity->getBoundingBox();

    if(bb.isValid()) {
      boundingBox.expand(bb[0]);
      boundingBox.expand(bb[1]);
    }
  }
}

void GlBoundingBoxSceneVisitor::visit(GlNode *glNode) {
  BoundingBox bb=glNode->getBoundingBox(inputData);

  boundingBox.expand(bb[0]);
  boundingBox.expand(bb[1]);
}

void GlBoundingBoxSceneVisitor::visit(GlEdge *glEdge) {
  BoundingBox bb=glEdge->getBoundingBox(inputData);

  boundingBox.expand(bb[0]);
  boundingBox.expand(bb[1]);
}
}
