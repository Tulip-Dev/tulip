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
#include <tulip/GlSelectSceneVisitor.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlLayer.h>
#include <tulip/GlLODCalculator.h>

using namespace std;

namespace tlp {

GlSelectSceneVisitor::GlSelectSceneVisitor(SelectionFlag flag,GlGraphInputData* inputData,GlLODCalculator *calculator)
  :selectionFlag(flag),inputData(inputData),calculator(calculator) {}

void GlSelectSceneVisitor::visit(GlSimpleEntity *entity) {
  if(selectionFlag==SelectSimpleEntities)
    calculator->addSimpleEntityBoundingBox(entity,entity->getBoundingBox());
}

void GlSelectSceneVisitor::visit(GlNode *glNode) {
  if(selectionFlag == SelectNodes)
    calculator->addNodeBoundingBox(glNode->id,glNode->getBoundingBox(inputData));
}

void GlSelectSceneVisitor::visit(GlEdge *glEdge) {
  if(selectionFlag == SelectEdges)
    calculator->addEdgeBoundingBox(glEdge->id,glEdge->getBoundingBox(inputData));
}

void GlSelectSceneVisitor::visit(GlLayer *layer) {
  calculator->beginNewCamera(&layer->getCamera());
}

}
