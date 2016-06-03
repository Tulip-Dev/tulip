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

#ifndef LABELSRENDERER_H
#define LABELSRENDERER_H

#include <tulip/BoundingBox.h>
#include <tulip/Graph.h>
#include <tulip/StringProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>

#include <vector>
#include <map>

#include <tulip/Camera.h>

namespace tlp {

class GlGraphInputData;

class TLP_GLES_SCOPE LabelsRenderer {

public :

  static LabelsRenderer *instance();

  static LabelsRenderer *instance(const std::string &canvasId);

  static void setCurrentCanvasId(const std::string &canvasId) {
    _currentCanvasId = canvasId;
  }

  void initFont(const std::string &fontFile);

  void addOrUpdateNodeLabel(const GlGraphInputData &inputData, tlp::node n);

  void removeNodeLabel(tlp::Graph *graph, tlp::node n);

  void setLabelsScaled(const bool labelsScaled) {
    _labelsScaled = labelsScaled;
  }

  bool labelsScaled() const {
    return _labelsScaled;
  }

  void setMinMaxSizes(float minSize, float maxSize) {
    _minSize = minSize;
    _maxSize = maxSize;
  }

  void setOcclusionTest(const bool occlusionTest) {
    _occlusionTest = occlusionTest;
  }

  void renderGraphNodesLabels(const GlGraphInputData &inputData, const Camera &camera, const tlp::Color &selectionColor);

  void renderOneLabel(const Camera &camera, const std::string &text, const tlp::BoundingBox &renderingBox,
                      const tlp::Color &labelColor = tlp::Color::Black, const std::string &fontFile = "");

  void clearGraphNodesLabelsRenderingData(tlp::Graph *graph);

  void setGraphNodesLabelsToRender(tlp::Graph *graph, const std::vector<tlp::node> &labelsToRender) {
    _labelsToRender[graph] = labelsToRender;
  }

private :

  void setFont(const std::string &fontFile);

  static std::map<std::string, LabelsRenderer *> _instances;
  static std::string _currentCanvasId;

  LabelsRenderer();

  float getTextAspectRatio(const std::string &text);

  tlp::BoundingBox getLabelRenderingBoxScaled(const tlp::BoundingBox &renderingBox, float textAspectRatio);

  std::map<std::string, int> _fontHandles;
  std::string _currentFont;

  std::map<tlp::Graph *, std::vector<tlp::node> > _labelsToRender;
  std::map<tlp::Graph *, std::map<tlp::node, float> > _nodeLabelAspectRatio;
  std::map<tlp::Graph *, std::map<tlp::node, unsigned int> > _nodeLabelNbLines;

  bool _labelsScaled;
  float _minSize, _maxSize;
  bool _occlusionTest;

};

}

#endif // LABELSRENDERER_H
