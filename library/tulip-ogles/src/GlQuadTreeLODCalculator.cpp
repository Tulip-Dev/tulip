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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <tulip/GlQuadTreeLODCalculator.h>
#include <tulip/GlUtils.h>
#include <tulip/QuadTree.h>
#include <tulip/GlEntity.h>
#include <tulip/GlLayer.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Matrix.h>
#include <tulip/LayoutProperty.h>
#include <tulip/PropertyInterface.h>

using namespace std;
using namespace tlp;

GlQuadTreeLODCalculator::GlQuadTreeLODCalculator() : _nodesQuadTree(nullptr), _edgesQuadTree(nullptr), _haveToCompute(true) {
#ifdef __EMSCRIPTEN__
  _haveToCompute = false;
#endif
}

GlQuadTreeLODCalculator::~GlQuadTreeLODCalculator() {
  std::map<GlLayer*, QuadTreeNode<GlEntity*> *>::iterator it = _glEntitiesQuadTree.begin();
  for (; it != _glEntitiesQuadTree.end() ; ++it) {
    delete it->second;
  }
  delete _nodesQuadTree;
  delete _edgesQuadTree;
}

void GlQuadTreeLODCalculator::setGraph(tlp::Graph *graph, GlGraphRenderingParameters *renderingParameters) {
  _haveToCompute = true;
  GlCPULODCalculator::setGraph(graph, renderingParameters);
}

void GlQuadTreeLODCalculator::initQuadTree() {
  std::map<GlLayer*, QuadTreeNode<GlEntity*> *>::iterator it = _glEntitiesQuadTree.begin();
  for (; it != _glEntitiesQuadTree.end() ; ++it) {
    delete it->second;
  }
  _glEntitiesQuadTree.clear();
  delete _nodesQuadTree;
  delete _edgesQuadTree;

  if (_sceneBoundingBox.isValid()) {
    _nodesQuadTree = new QuadTreeNode<node>(_sceneBoundingBox);
    _edgesQuadTree = new QuadTreeNode<edge>(_sceneBoundingBox);
  } else {
    _nodesQuadTree = nullptr;
    _edgesQuadTree = nullptr;
  }
}

void GlQuadTreeLODCalculator::clear() {
  GlCPULODCalculator::clear();
  initQuadTree();
}

void GlQuadTreeLODCalculator::setSceneBoundingBox(const tlp::BoundingBox &sceneBoundingBox) {
  GlCPULODCalculator::setSceneBoundingBox(sceneBoundingBox);
  initQuadTree();
}

void GlQuadTreeLODCalculator::addGlEntity(GlLayer *layer, GlEntity *glEntity) {
  if (_sceneBoundingBox.isValid() && _glEntitiesQuadTree.find(layer) == _glEntitiesQuadTree.end()) {
    _glEntitiesQuadTree[layer] = new QuadTreeNode<GlEntity*>(_sceneBoundingBox);
  }
  if (_sceneBoundingBox.isValid() && _glEntitiesQuadTree[layer]->getCellForElement(glEntity) == nullptr) {
    _glEntitiesQuadTree[layer]->insert(glEntity->getBoundingBox(), glEntity);
  }
  GlCPULODCalculator::addGlEntity(layer, glEntity);
}

void GlQuadTreeLODCalculator::removeGlEntity(GlLayer *layer, GlEntity *glEntity) {
  if (_glEntitiesQuadTree.find(layer) != _glEntitiesQuadTree.end()) {
    _glEntitiesQuadTree[layer]->remove(glEntity);
  }
}

void GlQuadTreeLODCalculator::removeLayer(GlLayer *layer) {
  if (_glEntitiesQuadTree.find(layer) != _glEntitiesQuadTree.end()) {
    delete _glEntitiesQuadTree[layer];
    _glEntitiesQuadTree.erase(layer);
  }
}

void GlQuadTreeLODCalculator::addNode(const tlp::node n) {
  GlCPULODCalculator::addNode(n);
  insertNodeInQuadTree(_nodesLODVector.back().boundingBox, n);
}

void GlQuadTreeLODCalculator::addEdge(const tlp::edge e) {
  GlCPULODCalculator::addEdge(e);
  insertEdgeInQuadTree(_edgesLODVector.back().boundingBox, e);
}

void GlQuadTreeLODCalculator::insertNodeInQuadTree(const tlp::BoundingBox &nodeBoundingBox, const tlp::node n) {
  _nodesQuadTree->insert(nodeBoundingBox, n);
}

void GlQuadTreeLODCalculator::insertEdgeInQuadTree(BoundingBox edgeBoundingBox, const tlp::edge e) {
  // This code is here to expand edge bounding box when we have an edge with direction (0,0,x)
  if(edgeBoundingBox[0][0] == edgeBoundingBox[1][0] && edgeBoundingBox[0][1] == edgeBoundingBox[1][1]) {
    edgeBoundingBox.expand(edgeBoundingBox[1]+Coord(0.01,0.01,0));
  }
  _edgesQuadTree->insert(edgeBoundingBox, e);
}

void GlQuadTreeLODCalculator::computeFor3DCamera(const Coord &eye, const Matrix<float, 4> &transformMatrix,
                                                 const Vector<int,4>& globalViewport, const Vector<int,4>& currentViewport) {

  // aX,aY : rotation on the camera in x and y
  Coord eyeCenter=camera->getCenter()-camera->getEyes();
  double aX=atan(eyeCenter[1]/eyeCenter[2]);
  double aY=atan(eyeCenter[0]/eyeCenter[2]);

  _glEntitiesLODVector.clear();
  _nodesLODVector.clear();
  _edgesLODVector.clear();

  MatrixGL invTransformMatrix(transformMatrix);
  invTransformMatrix.inverse();
  Coord pSrc = projectPoint(Coord(0,0,0), transformMatrix, globalViewport);

  BoundingBox cameraBoundingBox;

  // Project camera bounding box to known visible part of the quadtree
  pSrc[0] = currentViewport[0];
  pSrc[1] = (globalViewport[1] + globalViewport[3]) - (currentViewport[1] + currentViewport[3]);
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[1] = currentViewport[1]+currentViewport[3];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[0] = currentViewport[0]+currentViewport[2];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));
  pSrc[1] = currentViewport[1];
  cameraBoundingBox.expand(unprojectPoint(pSrc, invTransformMatrix, globalViewport));

  // Get result of quadtrees
#ifdef _OPENMP
#pragma omp parallel
#endif
  {
#ifdef _OPENMP
#pragma omp sections nowait
#endif
    {
#ifdef _OPENMP
#pragma omp section
#endif
      {
        if((_renderingEntitiesFlag & RenderingGlEntities)!=0) {
          std::map<GlLayer*, QuadTreeNode<GlEntity*> *>::iterator it = _glEntitiesQuadTree.begin();
          for (; it != _glEntitiesQuadTree.end() ; ++it) {
            vector<GlEntity*> resGlEntities;
            if(aX==0 && aY==0) {
              it->second->getElements(cameraBoundingBox,resGlEntities);
            }
            else {
              it->second->getElements(resGlEntities);
            }

            for(size_t i = 0 ; i < resGlEntities.size() ; ++i) {
              _glEntitiesLODVector[it->first].push_back(GlEntityLODUnit(resGlEntities[i]));
            }
          }
        }
      }
#ifdef _OPENMP
#pragma omp section
#endif
      {
        if(_graph && (_renderingEntitiesFlag & RenderingNodes)!=0) {
          vector<node> resNodes;
          resNodes.reserve(_graph->numberOfNodes());
          if(aX==0 && aY==0) {
            if(_nodesQuadTree)
              _nodesQuadTree->getElements(cameraBoundingBox,resNodes);
          }
          else {
            if(_nodesQuadTree)
              _nodesQuadTree->getElements(resNodes);
          }


          _nodesLODVector.reserve(resNodes.size());

          for(size_t i = 0 ; i < resNodes.size() ; ++i) {
            _nodesLODVector.push_back(NodeEntityLODUnit(resNodes[i], _nodesBBCache[resNodes[i]]));
          }
        }
      }
#ifdef _OPENMP
#pragma omp section
#endif
      {
        if(_graph && (_renderingEntitiesFlag & RenderingEdges)!=0) {
          vector<edge> resEdges;
          resEdges.reserve(_graph->numberOfEdges());
          if(aX==0 && aY==0) {
            if(_edgesQuadTree)
              _edgesQuadTree->getElements(cameraBoundingBox,resEdges);
          }
          else {
            if(_edgesQuadTree)
              _edgesQuadTree->getElements(resEdges);
          }

          for(size_t i = 0 ; i < resEdges.size() ; ++i) {
            _edgesLODVector.push_back(EdgeEntityLODUnit(resEdges[i], _edgesBBCache[resEdges[i]]));
          }
        }
      }
    }
  }

  GlCPULODCalculator::computeFor3DCamera(eye,transformMatrix,globalViewport,currentViewport);
}

