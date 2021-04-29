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
#include <set>

#include <tulip/GlQuadTreeLODCalculator.h>

#include <tulip/Matrix.h>
#include <tulip/QuadTree.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Camera.h>
#include <tulip/GlEntity.h>
#include <tulip/GlTools.h>
#include <tulip/GlScene.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlSceneObserver.h>
#include <tulip/ParallelTools.h>

using namespace std;

namespace tlp {

/*BoundingBox computeNewBoundingBox(const BoundingBox &box, const Coord &centerScene, double aX,
                                  double aY) {
  // compute a new bounding box : this bounding box is the rotation of the old bounding box
  Coord &&size = (box[1] - box[0]) / 2.f;
  Coord &&center = box[0] + size;
  // size = Coord(size.norm(),size.norm(),size.norm());
  size.fill(size.norm());
  center[0] = centerScene[0] + (cos(aY) * (center[0] - centerScene[0]));
  center[1] = centerScene[1] + (cos(aX) * (center[1] - centerScene[1]));

  return BoundingBox(center - size, center + size);
  }*/

GlQuadTreeLODCalculator::GlQuadTreeLODCalculator()
    : haveToCompute(true), haveToInitObservers(true),
      seBBIndex(2 * ThreadManager::getNumberOfThreads()),
      eBBOffset(ThreadManager::getNumberOfThreads()), currentGraph(nullptr),
      layoutProperty(nullptr), sizeProperty(nullptr), selectionProperty(nullptr) {
  threadSafe = true;
  // we have to deal with
  // ThreadManager::getNumberOfThreads() bounding boxes for nodes
  // ThreadManager::getNumberOfThreads() bounding boxes for edges
  // 1 bounding box for simple entities
  noBBCheck.assign(2 * ThreadManager::getNumberOfThreads() + 1, false);
  bbs.resize(2 * ThreadManager::getNumberOfThreads() + 1);
}

GlQuadTreeLODCalculator::~GlQuadTreeLODCalculator() {
  setHaveToCompute();
  clearCamerasObservers();

  for (auto it : nodesQuadTree)
    delete it;

  for (auto it : edgesQuadTree)
    delete it;

  for (auto it : entitiesQuadTree)
    delete it;
}

void GlQuadTreeLODCalculator::setScene(GlScene &scene) {
  // If we change scene, we have to remove observer on the graph and the scene
  // in the next rendering, we have to compute quadtree
  setHaveToCompute();
  GlLODCalculator::setScene(scene);
}

void GlQuadTreeLODCalculator::setInputData(const GlGraphInputData *newInputData) {
  setHaveToCompute();

  if (newInputData == nullptr) {
    currentCamera = nullptr;
    currentGraph = nullptr;
    layoutProperty = nullptr;
    sizeProperty = nullptr;
    selectionProperty = nullptr;
  }

  GlLODCalculator::setInputData(newInputData);
}

bool GlQuadTreeLODCalculator::needEntities() {
  if (inputData != nullptr) {
    // Checks if the properties in the GlGraphInputData have changed
    if (layoutProperty != inputData->getElementLayout() ||
        sizeProperty != inputData->getElementSize() ||
        selectionProperty != inputData->getElementSelected()) {
      // Remove observers on old properties
      removeObservers();
      // Reinit properties and listen them
      addObservers();
      // Need to recompute
      haveToCompute = true;
      haveToInitObservers = false;
    }
  }

  // Check if quadtree need entities
  if (haveToCompute) {
    if (inputData)
      oldParameters = *inputData->parameters;

    return true;
  }

  // Check if a camera have changed (diff between old backup camera and current camera)
  for (auto &it : layerToCamera) {
    if (it.first->getCamera().is3D()) {
      Camera &camera = it.first->getCamera();
      Camera &oldCamera = it.second;
      Coord &&unitCamera = camera.getEyes() - camera.getCenter();
      unitCamera /= unitCamera.norm();
      Coord &&unitOldCamera = oldCamera.getEyes() - oldCamera.getCenter();
      unitOldCamera /= unitOldCamera.norm();

      if (unitCamera != unitOldCamera) {
        haveToCompute = true;

        if (inputData)
          oldParameters = *inputData->parameters;

        return true;
      }
    }
  }

  if (inputData) {
    // Check visibility flags
    GlGraphRenderingParameters *newParameters = inputData->parameters;

    if (oldParameters.isDisplayEdges() != newParameters->isDisplayEdges() ||
        oldParameters.isDisplayMetaNodes() != newParameters->isDisplayMetaNodes() ||
        oldParameters.isDisplayNodes() != newParameters->isDisplayNodes() ||
        oldParameters.isViewNodeLabel() != newParameters->isViewNodeLabel() ||
        oldParameters.isViewEdgeLabel() != newParameters->isViewEdgeLabel() ||
        oldParameters.isViewMetaLabel() != newParameters->isViewMetaLabel()) {
      oldParameters = *inputData->parameters;
      haveToCompute = true;
      return true;
    }
  }

  return false;
}

void GlQuadTreeLODCalculator::setNeedEntities(bool) {
  setHaveToCompute();
}

void GlQuadTreeLODCalculator::addSimpleEntityBoundingBox(GlSimpleEntity *entity,
                                                         const BoundingBox &bb) {
  // same check as in GlCPULODCalculator::addSimpleEntityBoundingBox
  if (bb[0][0] != numeric_limits<float>::min()) {
    bbs[seBBIndex].expand(bb, noBBCheck[seBBIndex]);
    noBBCheck[seBBIndex] = true;
  }
  currentLayerLODUnit->simpleEntitiesLODVector.emplace_back(entity, bb);
}

void GlQuadTreeLODCalculator::addEdgeBoundingBox(unsigned int id, unsigned int pos,
                                                 const BoundingBox &bb) {
  auto ti = eBBOffset + ThreadManager::getThreadNumber();
  bbs[ti].expand(bb, noBBCheck[ti]);
  noBBCheck[ti] = true;
  currentLayerLODUnit->edgesLODVector[pos].init(id, pos, bb);
}

void GlQuadTreeLODCalculator::compute(const Vector<int, 4> &globalViewport,
                                      const Vector<int, 4> &currentViewport) {

  if (haveToCompute) {
    // if have to compute : rebuild quadtree

    if (haveToInitObservers) {
      addObservers();
      haveToInitObservers = false;
    }

    clearCamerasObservers();

    // Clear all vectors
    cameras.clear();
    layerToCamera.clear();
    simpleEntities.clear();

    for (auto it : nodesQuadTree)
      delete it;

    nodesQuadTree.clear();

    for (auto it : edgesQuadTree)
      delete it;

    edgesQuadTree.clear();

    for (auto it : entitiesQuadTree)
      delete it;

    entitiesQuadTree.clear();

    quadTreesVectorPosition = 0;
    const vector<pair<std::string, GlLayer *>> &layersVector = glScene->getLayersList();

    cameras.reserve(layersLODVector.size());

    for (auto &it : layersLODVector) {
      Camera *camera = it.camera;

      GlLayer *currentLayer = nullptr;

      for (auto &itL : layersVector) {
        if (&itL.second->getCamera() == camera) {
          currentLayer = itL.second;
          break;
        }
      }

      cameras.push_back(camera);

      if (currentLayer != nullptr) {
        layerToCamera.emplace(currentLayer, *camera);
      }

      Matrix<float, 4> transformMatrix;
      camera->getTransformMatrix(globalViewport, transformMatrix);

      Coord eye;

      if (camera->is3D()) {
        currentCamera = camera;
        eye = camera->getEyes() +
              (camera->getEyes() - camera->getCenter()) / float(camera->getZoomFactor());
        computeFor3DCamera(&it, eye, transformMatrix, globalViewport, currentViewport);
        quadTreesVectorPosition++;
      } else {
        simpleEntities.emplace_back(it.simpleEntitiesLODVector);
        computeFor2DCamera(&it, globalViewport, currentViewport);
      }

      glMatrixMode(GL_MODELVIEW);
    }

    initCamerasObservers();

    haveToCompute = false;

  } else {
    // if don't have to compute : use stored quadtree data

    layersLODVector.clear();

    quadTreesVectorPosition = 0;
    simpleEntitiesVectorPosition = 0;

    for (auto camera : cameras) {
      layersLODVector.emplace_back();
      LayerLODUnit *layerLODUnit = &(layersLODVector.back());
      layerLODUnit->camera = camera;

      Matrix<float, 4> transformMatrix;
      camera->getTransformMatrix(globalViewport, transformMatrix);

      Coord eye;

      if (camera->is3D()) {
        currentCamera = camera;
        eye = camera->getEyes() +
              (camera->getEyes() - camera->getCenter()) / float(camera->getZoomFactor());
        computeFor3DCamera(layerLODUnit, eye, transformMatrix, globalViewport, currentViewport);
        quadTreesVectorPosition++;
      } else {
        layerLODUnit->simpleEntitiesLODVector = simpleEntities[simpleEntitiesVectorPosition];
        computeFor2DCamera(layerLODUnit, globalViewport, currentViewport);
        simpleEntitiesVectorPosition++;
      }
    }
  }
}

void GlQuadTreeLODCalculator::computeFor3DCamera(LayerLODUnit *layerLODUnit, const Coord &eye,
                                                 const Matrix<float, 4> &transformMatrix,
                                                 const Vector<int, 4> &globalViewport,
                                                 const Vector<int, 4> &currentViewport) {

  // aX,aY : rotation on the camera in x and y
  Coord &&eyeCenter = currentCamera->getCenter() - currentCamera->getEyes();
  double aX = atan(eyeCenter[1] / eyeCenter[2]);
  double aY = atan(eyeCenter[0] / eyeCenter[2]);

  if (haveToCompute) {
    // Create quadtrees
    if (noBBCheck[seBBIndex]) // is bb for simple entities valid
      entitiesQuadTree.push_back(new QuadTreeNode<GlSimpleEntity *>(bbs[seBBIndex]));
    else
      entitiesQuadTree.push_back(nullptr);

    bool bbsOK = false;
    for (unsigned int i = 0; i < ThreadManager::getNumberOfThreads(); ++i) {
      if (noBBCheck[i]) {
        bbsOK = true;
        break;
      }
    }
    if (bbsOK) { // check validity of bbs for nodes
      // compute nodes dedicated bb
      BoundingBox bb(bbs[0]);

      for (unsigned int i = 1; i < eBBOffset; ++i) {
        if (noBBCheck[i])
          bb.expand(bbs[i], true);
      }

      nodesQuadTree.push_back(new QuadTreeNode<std::pair<unsigned int, unsigned int>>(bb));
    } else {
      nodesQuadTree.push_back(nullptr);
    }

    bbsOK = false;
    for (unsigned int i = 0; i < ThreadManager::getNumberOfThreads(); ++i) {
      if (noBBCheck[eBBOffset + i]) {
        bbsOK = true;
        break;
      }
    }
    if (bbsOK) { // check validity of bbs for edges
      // compute edges dedicated bb
      BoundingBox bb(bbs[eBBOffset]);

      for (unsigned int i = eBBOffset + 1; i < seBBIndex; ++i) {
        if (noBBCheck[i])
          bb.expand(bbs[i], true);
      }

      edgesQuadTree.push_back(new QuadTreeNode<std::pair<unsigned int, unsigned int>>(bb));
    } else {
      edgesQuadTree.push_back(nullptr);
    }

    // Add entities in quadtrees
    size_t nbSimples = layerLODUnit->simpleEntitiesLODVector.size();
    size_t nbNodes = layerLODUnit->nodesLODVector.size();
    size_t nbEdges = layerLODUnit->edgesLODVector.size();
    auto thrdF1 = [&]() {
      for (size_t i = 0; i < nbSimples; ++i) {
        const auto &entity = layerLODUnit->simpleEntitiesLODVector[i];
        entitiesQuadTree[quadTreesVectorPosition]->insert(entity.boundingBox, entity.entity);
      }
    };
    auto thrdF2 = [&]() {
      for (size_t i = 0; i < nbNodes; ++i) {
        const auto &entity = layerLODUnit->nodesLODVector[i];
        nodesQuadTree[quadTreesVectorPosition]->insert(entity.boundingBox,
                                                       std::make_pair(entity.id, entity.pos));
      }
    };
    auto thrdF3 = [&]() {
      for (size_t i = 0; i < nbEdges; ++i) {
        // This code is here to expand edge bounding box when we have an edge with direction
        // (0,0,x)
        auto &entity = layerLODUnit->edgesLODVector[i];
        if (entity.boundingBox[0][0] == entity.boundingBox[1][0] &&
            entity.boundingBox[0][1] == entity.boundingBox[1][1]) {
          entity.boundingBox.expand(entity.boundingBox[1] + Coord(0.01f, 0.01f, 0));
        }

        edgesQuadTree[quadTreesVectorPosition]->insert(entity.boundingBox,
                                                       std::make_pair(entity.id, entity.pos));
      }
    };
    TLP_PARALLEL_SECTIONS(thrdF1, thrdF2, thrdF3);

    layerLODUnit->simpleEntitiesLODVector.clear();
    layerLODUnit->nodesLODVector.clear();
    layerLODUnit->edgesLODVector.clear();
  }

  MatrixGL invTransformMatrix(transformMatrix);
  invTransformMatrix.inverse();
  Coord &&pSrc = projectPoint(Coord(0, 0, 0), transformMatrix, globalViewport);

  Vector<int, 4> transformedViewport = currentViewport;
  transformedViewport[1] = globalViewport[3] - (currentViewport[1] + currentViewport[3]);

  // Project camera bondinx box to know visible part of the quadtree
  pSrc[0] = transformedViewport[0];
  pSrc[1] =
      (globalViewport[1] + globalViewport[3]) - (transformedViewport[1] + transformedViewport[3]);
  BoundingBox cameraBoundingBox;
  cameraBoundingBox.init(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[1] = transformedViewport[1] + transformedViewport[3];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport), true);
  pSrc[0] = transformedViewport[0] + transformedViewport[2];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport), true);
  pSrc[1] = transformedViewport[1];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport), true);

  int ratio;

  if (currentViewport[2] > currentViewport[3])
    ratio = currentViewport[2];
  else
    ratio = currentViewport[3];

  vector<std::pair<unsigned int, unsigned int>> resNodes;
  vector<std::pair<unsigned int, unsigned int>> resEdges;
  vector<GlSimpleEntity *> resEntities;

  // Get result of quadtrees
  auto thrdF1 = [&]() {
    if ((renderingEntitiesFlag & RenderingNodes) != 0) {
      auto &quadTree = nodesQuadTree[quadTreesVectorPosition];
      if (quadTree) {
        if (aX == 0 && aY == 0) {
          if ((renderingEntitiesFlag & RenderingWithoutRemove) == 0) {
            quadTree->getElementsWithRatio(cameraBoundingBox, resNodes, ratio);
          } else {
            quadTree->getElements(cameraBoundingBox, resNodes);
          }
        } else {
          quadTree->getElements(resNodes);
        }
      }
    }
    size_t nbRes = resNodes.size();
    layerLODUnit->nodesLODVector.resize(nbRes);
  };
  auto thrdF2 = [&]() {
    if ((renderingEntitiesFlag & RenderingEdges) != 0) {
      auto &quadTree = edgesQuadTree[quadTreesVectorPosition];
      if (quadTree) {
        if (aX == 0 && aY == 0) {
          if ((renderingEntitiesFlag & RenderingWithoutRemove) == 0) {
            quadTree->getElementsWithRatio(cameraBoundingBox, resEdges, ratio);
          } else {
            quadTree->getElements(cameraBoundingBox, resEdges);
          }
        } else {
          quadTree->getElements(resEdges);
        }
      }
    }
    size_t nbRes = resEdges.size();
    layerLODUnit->edgesLODVector.resize(nbRes);
  };
  auto thrdF3 = [&]() {
    if ((renderingEntitiesFlag & RenderingSimpleEntities) != 0) {
      auto &quadTree = entitiesQuadTree[quadTreesVectorPosition];
      if (quadTree) {
        if (aX == 0 && aY == 0) {
          if ((renderingEntitiesFlag & RenderingWithoutRemove) == 0)
            quadTree->getElementsWithRatio(cameraBoundingBox, resEntities, ratio);
          else
            quadTree->getElements(cameraBoundingBox, resEntities);
        } else {
          quadTree->getElements(resEntities);
        }
      }
    }
    size_t nbRes = resEntities.size();
    for (size_t i = 0; i < nbRes; ++i) {
      layerLODUnit->simpleEntitiesLODVector.emplace_back(resEntities[i],
                                                         resEntities[i]->getBoundingBox());
    }
  };
  TLP_PARALLEL_SECTIONS(thrdF1, thrdF2, thrdF3);
  TLP_PARALLEL_MAP_INDICES(resNodes.size(), [&](unsigned int i) {
    const auto &res = resNodes[i];
    GlNode glNode(res.first, res.second);
    layerLODUnit->nodesLODVector[i].init(res.first, res.second, glNode.getBoundingBox(inputData));
  });
  TLP_PARALLEL_MAP_INDICES(resEdges.size(), [&](unsigned int i) {
    const auto &res = resEdges[i];
    GlEdge glEdge(res.first, res.second);
    layerLODUnit->edgesLODVector[i].init(res.first, res.second, glEdge.getBoundingBox(inputData));
  });

  GlCPULODCalculator::computeFor3DCamera(layerLODUnit, eye, transformMatrix, globalViewport,
                                         currentViewport);
}

void GlQuadTreeLODCalculator::removeObservers() {
  if (inputData) {
    if (currentGraph)
      currentGraph->removeListener(this);

    if (layoutProperty) {
      layoutProperty->removeListener(this);
      layoutProperty = nullptr;
    }

    if (sizeProperty) {
      sizeProperty->removeListener(this);
      sizeProperty = nullptr;
    }

    if (selectionProperty) {
      selectionProperty->removeListener(this);
      selectionProperty = nullptr;
    }
  }

  if (glScene)
    glScene->removeListener(this);
}

void GlQuadTreeLODCalculator::addObservers() {
  if (inputData) {
    currentGraph = inputData->getGraph();
    currentGraph->addListener(this);

    layoutProperty = inputData->getElementLayout();

    if (layoutProperty != nullptr) {
      layoutProperty->addListener(this);
    }

    sizeProperty = inputData->getElementSize();

    if (sizeProperty != nullptr) {
      sizeProperty->addListener(this);
    }

    selectionProperty = inputData->getElementSelected();

    if (selectionProperty != nullptr) {
      selectionProperty->addListener(this);
    }
  }

  if (glScene)
    glScene->addListener(this);
}

void GlQuadTreeLODCalculator::update(PropertyInterface *property) {
  if (property == inputData->getElementLayout() || property == inputData->getElementSize() ||
      property == inputData->getElementSelected())
    setHaveToCompute();
}

void GlQuadTreeLODCalculator::treatEvent(const Event &ev) {
  const GlSceneEvent *sceneEv = dynamic_cast<const GlSceneEvent *>(&ev);

  if (sceneEv) {
    setHaveToCompute();
  } else if (typeid(ev) == typeid(GraphEvent)) {
    const GraphEvent *graphEvent = static_cast<const GraphEvent *>(&ev);

    switch (graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_DEL_NODE:
    case GraphEvent::TLP_DEL_EDGE:
      setHaveToCompute();
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      const PropertyInterface *property =
          inputData->getGraph()->getProperty(graphEvent->getPropertyName());

      if (property == inputData->getElementLayout() || property == inputData->getElementSize()) {
        setHaveToCompute();
        removeObservers();
        addObservers();
      }

      break;
    }

    default:
      break;
    }
  } else if (typeid(ev) == typeid(PropertyEvent)) {
    const PropertyEvent *propertyEvent = static_cast<const PropertyEvent *>(&ev);
    PropertyInterface *property = propertyEvent->getProperty();

    switch (propertyEvent->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
      update(property);
      break;

    default:
      break;
    }
  } else if (ev.type() == Event::TLP_DELETE) {
    if (dynamic_cast<Camera *>(ev.sender())) {

      for (auto it = cameras.begin(); it != cameras.end(); ++it) {
        if (*it == dynamic_cast<Camera *>(ev.sender())) {
          (*it)->removeListener(this);
          cameras.erase(it);
          break;
        }
      }

      haveToCompute = true;
    }

    if (dynamic_cast<tlp::Graph *>(ev.sender())) {
      clear();
      setInputData(nullptr);
    }

    PropertyInterface *property;

    if ((property = dynamic_cast<PropertyInterface *>(ev.sender()))) {
      if (property == layoutProperty) {
        layoutProperty = nullptr;
      } else if (property == sizeProperty) {
        sizeProperty = nullptr;
      } else if (property == selectionProperty) {
        selectionProperty = nullptr;
      }
    }
  }
}

void GlQuadTreeLODCalculator::initCamerasObservers() {
  set<Camera *> treatedCameras;

  for (auto camera : cameras) {
    if (treatedCameras.find(camera) == treatedCameras.end()) {
      treatedCameras.insert(camera);
      camera->addListener(this);
    }
  }
}

void GlQuadTreeLODCalculator::clearCamerasObservers() {
  set<Camera *> treatedCameras;

  for (auto camera : cameras) {
    if (treatedCameras.find(camera) == treatedCameras.end()) {
      treatedCameras.insert(camera);
      camera->removeListener(this);
    }
  }
}

void GlQuadTreeLODCalculator::setHaveToCompute() {
  if (haveToCompute)
    return;

  GlQuadTreeLODCalculator *attachedQuadTreeLODCalculator =
      dynamic_cast<GlQuadTreeLODCalculator *>(attachedLODCalculator);

  if (attachedQuadTreeLODCalculator)
    attachedQuadTreeLODCalculator->setHaveToCompute();

  haveToCompute = true;
  haveToInitObservers = true;
  removeObservers();
}
} // namespace tlp
