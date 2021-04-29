/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/GlCPULODCalculator.h>
#include <tulip/GlGraphInputData.h>

#include <tulip/Matrix.h>
#include <tulip/Camera.h>
#include <tulip/GlTools.h>
#include <tulip/ParallelTools.h>

using namespace std;

namespace tlp {

GlCPULODCalculator::GlCPULODCalculator() : computeEdgesLOD(true), noBBCheck(false) {
  threadSafe = true;
  noBBCheck.assign(ThreadManager::getNumberOfThreads(), false);
  bbs.resize(ThreadManager::getNumberOfThreads());
}

GlCPULODCalculator::~GlCPULODCalculator() {}

void GlCPULODCalculator::beginNewCamera(Camera *camera) {
  // add a new layerLODUnit and set camera pointer
  layersLODVector.emplace_back();
  currentLayerLODUnit = &layersLODVector.back();
  currentLayerLODUnit->camera = camera;
}

void GlCPULODCalculator::addSimpleEntityBoundingBox(GlSimpleEntity *entity, const BoundingBox &bb) {
  assert(bb.isValid());

  // This code is here to prevent adding entities in percentage
  //  If you look at the Gl2DRect, you can see an option inPercent,
  //   if this entity is inPercent we cannot compute bounding box, so we create the biggest possible
  //   bounding box
  //   and here we don't add this false bounding box to the scene bounding box
  //   TODO: See if we can change the bounding box compute in Gl2DRect
  if (bb[0][0] != numeric_limits<float>::min()) {
    auto ti = ThreadManager::getThreadNumber();
    bbs[ti].expand(bb, noBBCheck[ti]);
    noBBCheck[ti] = true;
  }

  currentLayerLODUnit->simpleEntitiesLODVector.emplace_back(entity, bb);
}
void GlCPULODCalculator::addNodeBoundingBox(unsigned int id, unsigned int pos,
                                            const BoundingBox &bb) {
  auto ti = ThreadManager::getThreadNumber();
  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
  currentLayerLODUnit->nodesLODVector[pos].init(id, pos, bb);
}
void GlCPULODCalculator::addEdgeBoundingBox(unsigned int id, unsigned int pos,
                                            const BoundingBox &bb) {
  auto ti = ThreadManager::getThreadNumber();
  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
  currentLayerLODUnit->edgesLODVector[pos].init(id, pos, bb);
}

BoundingBox GlCPULODCalculator::getSceneBoundingBox() {
  BoundingBox bb(bbs[0]);

  for (unsigned int i = 1; i < bbs.size(); ++i)
    if (noBBCheck[i])
      bb.expand(bbs[i], true);
  return bb;
}

void GlCPULODCalculator::reserveMemoryForGraphElts(unsigned int nbNodes, unsigned int nbEdges) {
  currentLayerLODUnit->nodesLODVector.resize(nbNodes);
  currentLayerLODUnit->edgesLODVector.resize(nbEdges);
}

void GlCPULODCalculator::compute(const Vector<int, 4> &globalViewport,
                                 const Vector<int, 4> &currentViewport) {

  for (auto it = layersLODVector.begin(); it != layersLODVector.end(); ++it) {
    Camera *camera = it->camera;

    Matrix<float, 4> transformMatrix;
    camera->getTransformMatrix(globalViewport, transformMatrix);

    if (camera->is3D()) {
      Coord &&eye = camera->getEyes() +
                    (camera->getEyes() - camera->getCenter()) / float(camera->getZoomFactor());
      computeFor3DCamera(&(*it), eye, transformMatrix, globalViewport, currentViewport);
    } else {
      computeFor2DCamera(&(*it), globalViewport, currentViewport);
    }

    glMatrixMode(GL_MODELVIEW);
  }
}

void GlCPULODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit, const Coord &eye,
                                            const Matrix<float, 4> &transformMatrix,
                                            const Vector<int, 4> &globalViewport,
                                            const Vector<int, 4> &currentViewport) {

  if ((renderingEntitiesFlag & RenderingSimpleEntities) != 0) {
    auto nb = layerLODUnit->simpleEntitiesLODVector.size();
    TLP_PARALLEL_MAP_INDICES(nb, [&](unsigned int i) {
      layerLODUnit->simpleEntitiesLODVector[i].lod =
          calculateAABBSize(layerLODUnit->simpleEntitiesLODVector[i].boundingBox, eye,
                            transformMatrix, globalViewport, currentViewport);
    });
  }

  if ((renderingEntitiesFlag & RenderingNodes) != 0) {
    auto nb = layerLODUnit->nodesLODVector.size();
    TLP_PARALLEL_MAP_INDICES(nb, [&](unsigned int i) {
      layerLODUnit->nodesLODVector[i].lod =
          calculateAABBSize(layerLODUnit->nodesLODVector[i].boundingBox, eye, transformMatrix,
                            globalViewport, currentViewport);
    });
  }

  if ((renderingEntitiesFlag & RenderingEdges) != 0) {
    auto nb = layerLODUnit->edgesLODVector.size();

    if (computeEdgesLOD) {
      TLP_PARALLEL_MAP_INDICES(nb, [&](unsigned int i) {
        layerLODUnit->edgesLODVector[i].lod =
            calculateAABBSize(layerLODUnit->edgesLODVector[i].boundingBox, eye, transformMatrix,
                              globalViewport, currentViewport);
      });
    } else {
      TLP_PARALLEL_MAP_INDICES(nb,
                               [&](unsigned int i) { layerLODUnit->edgesLODVector[i].lod = 10; });
    }
  }
}

void GlCPULODCalculator::computeFor2DCamera(LayerLODUnit *layerLODUnit,
                                            const Vector<int, 4> &globalViewport,
                                            const Vector<int, 4> &currentViewport) {

  for (auto it = layerLODUnit->simpleEntitiesLODVector.begin();
       it != layerLODUnit->simpleEntitiesLODVector.end(); ++it) {
    it->lod = calculate2DLod(it->boundingBox, globalViewport, currentViewport);
  }

  for (auto it = layerLODUnit->nodesLODVector.begin(); it != layerLODUnit->nodesLODVector.end();
       ++it) {
    it->lod = calculate2DLod(it->boundingBox, globalViewport, currentViewport);
  }

  for (auto it = layerLODUnit->edgesLODVector.begin(); it != layerLODUnit->edgesLODVector.end();
       ++it) {
    it->lod = calculate2DLod(it->boundingBox, globalViewport, currentViewport);
  }
}
} // namespace tlp
