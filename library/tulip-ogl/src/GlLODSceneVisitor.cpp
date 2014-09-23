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
#include <tulip/GlLODSceneVisitor.h>

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlLayer.h>
#include <tulip/GlLODCalculator.h>

using namespace std;

namespace tlp {

void GlLODSceneVisitor::visit(GlSimpleEntity *entity) {
  if(entity->isVisible())
    calculator->addSimpleEntityBoundingBox(entity, entity->getBoundingBox());
}

void GlLODSceneVisitor::visit(GlNode *glNode) {
  calculator->addNodeBoundingBox(glNode->id,glNode->getBoundingBox(inputData));
}

void GlLODSceneVisitor::visit(GlEdge *glEdge) {
  calculator->addEdgeBoundingBox(glEdge->id,glEdge->getBoundingBox(inputData));
}

void GlLODSceneVisitor::visit(GlLayer *layer) {
  calculator->beginNewCamera(&layer->getCamera());
}

void GlLODSceneVisitor::reserveMemoryForNodes(unsigned int numberOfNodes) {
  calculator->reserveMemoryForNodes(numberOfNodes);
}

void GlLODSceneVisitor::reserveMemoryForEdges(unsigned int numberOfEdges) {
  calculator->reserveMemoryForEdges(numberOfEdges);
}
}
