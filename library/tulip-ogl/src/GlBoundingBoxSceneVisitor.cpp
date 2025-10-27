/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/ParallelTools.h>

using namespace std;

namespace tlp {

GlBoundingBoxSceneVisitor::GlBoundingBoxSceneVisitor(GlGraphInputData *inputData)
    : inputData(inputData) {
  threadSafe = true;
  noBBCheck.assign(ThreadManager::getNumberOfThreads(), false);
  bbs.resize(ThreadManager::getNumberOfThreads());
}

BoundingBox GlBoundingBoxSceneVisitor::getBoundingBox() {
  BoundingBox bb(bbs[0]);

  for (unsigned int i = 1; i < bbs.size(); ++i)
    if (noBBCheck[i])
      bb.expand(bbs[i], true);
  return bb;
}

void GlBoundingBoxSceneVisitor::visit(GlSimpleEntity *entity) {
  if (entity->isVisible()) {
    BoundingBox &&bb = entity->getBoundingBox();

    if (bb.isValid()) {
      auto ti = ThreadManager::getThreadNumber();
      bbs[ti].expand(bb, noBBCheck[ti]);
      noBBCheck[ti] = true;
    }
  }
}

void GlBoundingBoxSceneVisitor::visit(GlNode *glNode) {
  BoundingBox &&bb = glNode->getBoundingBox(inputData);
  auto ti = ThreadManager::getThreadNumber();

  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
}

void GlBoundingBoxSceneVisitor::visit(GlEdge *glEdge) {
  BoundingBox &&bb = glEdge->getBoundingBox(inputData);
  auto ti = ThreadManager::getThreadNumber();

  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
}
} // namespace tlp
