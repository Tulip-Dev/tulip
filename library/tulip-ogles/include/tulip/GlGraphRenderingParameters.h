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

#ifndef GLGRAPHRENDERINGPARAMETERS_H
#define GLGRAPHRENDERINGPARAMETERS_H

#include <tulip/Color.h>
#include <tulip/DataSet.h>
#include <tulip/Observable.h>

namespace tlp {

class NumericProperty;
class BooleanProperty;

class GlGraph;

class TLP_GLES_SCOPE GlGraphRenderingParameters : public tlp::Observable {

public:
  GlGraphRenderingParameters();

  void setGlGraph(GlGraph *glGraph);
  GlGraph *glGraph() const;

  tlp::DataSet getParameters() const;
  void setParameters(const tlp::DataSet &);

  void setDisplayEdgesExtremities(const bool state);
  bool displayEdgesExtremities() const;

  void setDisplayNodesLabels(const bool state);
  bool displayNodesLabels() const;

  void setDisplayEdgesLabels(const bool state);
  bool displayEdgesLabels() const;

  void setInterpolateEdgesColors(const bool state);
  bool interpolateEdgesColors() const;

  void setInterpolateEdgesSizes(const bool state);
  bool interpolateEdgesSizes() const;

  void setDisplayMetaNodesLabels(const bool state);
  bool displayMetaNodesLabels() const;

  void setDisplayEdges(const bool state);
  bool displayEdges() const;

  void setDisplayNodes(const bool state);
  bool displayNodes() const;

  void setDisplayMetaNodes(const bool state);
  bool displayMetaNodes() const;

  void setNodesStencil(const int stencil);
  int nodesStencil() const;

  void setMetaNodesStencil(const int stencil);
  int metaNodesStencil() const;

  void setEdgesStencil(const int stencil);
  int edgesStencil() const;

  void setNodesLabelsStencil(const int stencil);
  int nodesLabelsStencil() const;

  void setMetaNodesLabelsStencil(const int stencil);
  int metaNodesLabelsStencil() const;

  void setEdgesLabelsStencil(const int stencil);
  int edgesLabelsStencil() const;

  void setElementsOrdered(const bool state);
  bool elementsOrdered() const;

  void setElementOrderedDescending(const bool state);
  bool elementsOrderedDescending() const;

  void setElementsOrderingProperty(tlp::NumericProperty *property);
  tlp::NumericProperty *elementsOrderingProperty() const;

  void setElementsZOrdered(const bool state);
  bool elementsZOrdered() const;

  void setEdges3D(const bool state);
  bool edges3D() const;

  void setMaxEdgesSizesToNodesSizes(const bool b);
  bool maxEdgesSizesToNodesSizes() const;

  void setSelectionColor(const tlp::Color &color);
  tlp::Color selectionColor() const;

  void setLabelsFixedFontSize(bool state);
  bool labelsFixedFontSize() const;

  void setLabelsScaled(bool state);
  bool labelsScaled() const;

  void setLabelsDensity(int density);
  int labelsDensity() const;

  void setMinSizeOfLabels(float size);
  float minSizeOfLabels() const;

  void setMaxSizeOfLabels(float size);
  float maxSizeOfLabels() const;

  void setBillboardedNodes(bool billboardedNodes);
  bool billboardedNodes() const;

  void setDisplayFilteringProperty(tlp::BooleanProperty *filteringProperty);
  tlp::BooleanProperty *displayFilteringProperty() const;

  void setBypassLodSystem(bool bypassLodSystem);
  bool bypassLodSystem() const;

  GlGraphRenderingParameters &operator=(const GlGraphRenderingParameters &params);

private:
  GlGraph *_glGraph;
  bool _displayEdgesExtremities;
  bool _displayNodesLabels;
  bool _displayEdgesLabels;
  bool _displayMetaNodesLabels;
  bool _elementsOrdered;
  bool _elementsOrderedDescending;
  bool _interpolateEdgesColors;
  bool _edges3D;
  bool _interpolateEdgesSizes;
  bool _displayEdges;
  bool _displayNodes;
  bool _displayMetaNodes;
  bool _elementsZOrdered;
  int _nodesStencil;
  int _metaNodesStencil;
  int _edgesStencil;
  int _nodesLabelsStencil;
  int _metaNodesLabelsStencil;
  int _edgesLabelsStencil;
  bool _labelsScaled;
  float _labelsMinSize;
  float _labelsMaxSize;
  int _labelsDensity;
  bool _billboardedNodes;
  bool _maxEdgesSizesToNodesSizes;
  tlp::Color _selectionColor;
  tlp::BooleanProperty *_displayFilteringProperty;
  tlp::NumericProperty *_elementsOrderingProperty;
  bool _bypassLodSystem;
  bool _labelsFixedFontSize;
};

class TLP_GLES_SCOPE GlGraphRenderingParametersEvent : public tlp::Event {

public:
  enum GlGraphRenderingParametersEventType {
    RENDERING_PARAMETERS_MODIFIED = 0,
    DISPLAY_EDGES_EXTREMITIES_TOGGLED,
    INTERPOLATE_EDGES_COLORS_TOGGLED,
    INTERPOLATE_EDGES_SIZES_TOGGLED
  };

  GlGraphRenderingParametersEvent(GlGraphRenderingParameters *renderingParameters, GlGraphRenderingParametersEventType eventType)
      : tlp::Event(*renderingParameters, tlp::Event::TLP_MODIFICATION), _eventType(eventType) {
  }

  GlGraphRenderingParametersEventType getType() const {
    return _eventType;
  }

  GlGraphRenderingParameters *getRenderingParameters() const {
    return static_cast<GlGraphRenderingParameters *>(sender());
  }

private:
  GlGraphRenderingParametersEventType _eventType;
};

class TLP_GLES_SCOPE GlDefaultSelectionColorManager {
public:
  static void setManager(GlDefaultSelectionColorManager *);
  static tlp::Color getDefaultSelectionColor();
  virtual tlp::Color defaultSelectionColor() = 0;

private:
  static GlDefaultSelectionColorManager *manager;
};
}

#endif // GLGRAPHRENDERINGPARAMETERS_H
