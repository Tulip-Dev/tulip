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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLGRAPH_H
#define GLGRAPH_H

#include <tulip/Graph.h>
#include <tulip/BoundingBox.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/TulipViewSettings.h>

#include <vector>
#include <map>
#include <set>

#include <tulip/GlEntity.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>

namespace tlp {

class BooleanProperty;
class ColorProperty;
class DoubleProperty;
class IntegerProperty;
class LayoutProperty;
class SizeProperty;
class StringProperty;

class Glyph;
class LabelsRenderer;
class GlShaderProgram;
class GlBuffer;
class GlLODCalculator;

class TLP_GLES_SCOPE GlGraph : public GlEntity {

public:
  GlGraph(tlp::Graph *graph = nullptr, GlLODCalculator *lodCalculator = nullptr);

  ~GlGraph();

  void setGraph(tlp::Graph *graph);

  tlp::Graph *getGraph() const {
    return _graph;
  }

  void draw(const Camera &camera, const Light &light, bool pickingMode = false);

  bool pickNodesAndEdges(const Camera &camera, const int x, const int y, const int width, const int height, std::vector<tlp::node> &selectedNodes,
                         std::vector<tlp::edge> &selectedEdges, bool singleSelection = false);

  bool pickNodeOrEdge(const Camera &camera, const int x, const int y, tlp::node &pickedNode, tlp::edge &pickedEdge);

  void treatEvents(const std::vector<tlp::Event> &);

  void treatEvent(const tlp::Event &message);

  void computeGraphBoundingBox();

  void clearEdgesData();
  void addEdgeData(const tlp::edge e);
  void uploadEdgesData();
  void prepareEdgesData();

  void initObservers();
  void clearObservers();

  GlLODCalculator *getLODCalculator() const {
    return _lodCalculator;
  }

  GlGraphInputData &getInputData() {
    return _inputData;
  }

  GlGraphRenderingParameters &getRenderingParameters() {
    return _renderingParameters;
  }

  void setRenderingParameters(const GlGraphRenderingParameters &renderingParameters);

private:
  void getEdgeExtremityData(tlp::edge e, bool srcGlyph, tlp::Coord &position, tlp::Size &size, tlp::Vec4f &rotationAxisAndAngle);

  void renderNodes(const Camera &camera, const Light &light);
  void renderNodesGlow(const std::vector<tlp::node> &nodes, const Camera &camera, const Light &light);
  void renderMetaNodes(const std::vector<tlp::node> &metaNodes, const Camera &camera, const Light &light);
  void renderPointsNodesAndEdges(const Camera &camera, const std::vector<tlp::node> &pointsNodes, const std::vector<tlp::edge> &pointsEdges);

  void renderEdges(const Camera &camera, const Light &light, const std::vector<tlp::edge> &edges, bool lineMode, bool billboard = false);

  void renderEdgeExtremities(const Camera &camera, const Light &light, const tlp::edge &e);

  void setGraphElementsPickingMode(const bool pickingMode) {
    _graphElementsPickingMode = pickingMode;
  }

  void prepareEdgeData(tlp::edge e);

  tlp::Size getEdgeSize(tlp::edge e);

  void initLODCalculator();

  tlp::Graph *_graph = nullptr;

  GlShaderProgram *_flatShader = nullptr;

  tlp::BoundingBox _sceneBoundingBox;

  std::map<tlp::edge, std::vector<tlp::Vec3f>> _edgePoints;
  std::map<tlp::edge, std::pair<tlp::Vec3f, tlp::Vec3f>> _edgeAnchors;
  std::map<tlp::edge, std::vector<unsigned int>> _edgeLineVerticesIndices;

  GlBuffer *_edgeRenderingDataBuffer = nullptr;
  GlBuffer *_edgeIndicesBuffer = nullptr;
  GlBuffer *_curveEdgeRenderingDataBuffer = nullptr;
  GlBuffer *_curveEdgeIndicesBuffer = nullptr;
  GlBuffer *_edgeLineRenderingDataBuffer = nullptr;
  GlBuffer *_edgeLineRenderingIndicesBuffer = nullptr;

  std::map<int, GlShaderProgram *> _edgesShaders;

  GlShaderProgram *getEdgeShader(int edgeShape);

  GlBuffer *_pointsDataBuffer = nullptr;

  std::vector<std::pair<int, std::vector<tlp::node>>> _nodesGlyphs;

  LabelsRenderer *_labelsRenderer = nullptr;

  bool _graphElementsPickingMode = false;

  std::set<tlp::edge> _edgesToUpdate;

  std::set<tlp::edge> _edgesToDiscard;
  std::set<tlp::node> _nodesToDiscard;

  GlLODCalculator *_lodCalculator;

  std::map<tlp::edge, std::vector<tlp::Vec3f>> _srcEdgeExtremitiesData;
  std::map<tlp::edge, std::vector<tlp::Vec3f>> _tgtEdgeExtremitiesData;

  size_t _maxEdgePoints;

  tlp::BoundingBox _lastGraphBoundingBox;

  GlGraphInputData _inputData;
  GlGraphRenderingParameters _renderingParameters;

  bool _updateLODCalculator = true;
  bool _edgesDataNeedUpload = true;

  std::set<PropertyInterface *> _observedProperties;
};
}

#endif // GLGRAPH_H
