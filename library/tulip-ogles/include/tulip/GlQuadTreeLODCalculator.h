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

#ifndef GLQUADTREELODCALCULATOR_H
#define GLQUADTREELODCALCULATOR_H

#include <map>
#include <vector>

#include <tulip/Observable.h>

#include <tulip/GlCPULODCalculator.h>
#include <tulip/Camera.h>

namespace tlp {

class PropertyInterface;
class Graph;
class GlEntity;

template <class TYPE> class QuadTreeNode;

class TLP_GLES_SCOPE GlQuadTreeLODCalculator : public GlCPULODCalculator {

public:

  GlQuadTreeLODCalculator();
  ~GlQuadTreeLODCalculator();

  void setGraph(tlp::Graph *graph, GlGraphRenderingParameters *renderingParameters);

  void computeFor3DCamera(const tlp::Coord &eye,
                          const tlp::MatrixGL &transformMatrix,
                          const tlp::Vec4i &globalViewport,
                          const tlp::Vec4i &currentViewport);

  void setSceneBoundingBox(const tlp::BoundingBox &sceneBoundingBox);

  bool haveToCompute() const {
    return _haveToCompute;
  }

  void setHaveToCompute(bool haveToCompute) {
    _haveToCompute = haveToCompute;
  }

  void addGlEntity(GlLayer *layer, GlEntity *glEntity);

  void removeGlEntity(GlLayer *layer, GlEntity *glEntity);

  void removeLayer(GlLayer *layer);

  void addNode(const tlp::node n);

  void addEdge(const tlp::edge e);

  void clear();

private :

  void initQuadTree();

  void insertNodeInQuadTree(const tlp::BoundingBox &nodeBoundingBox, const tlp::node n);
  void insertEdgeInQuadTree(tlp::BoundingBox edgeBoundingBox, const tlp::edge e);

  std::map<GlLayer*, QuadTreeNode<GlEntity*> *> _glEntitiesQuadTree;
  QuadTreeNode<tlp::node> *_nodesQuadTree;
  QuadTreeNode<tlp::edge> *_edgesQuadTree;

  bool _haveToCompute;

};

}

#endif // GLQUADTREELODCALCULATOR_H
