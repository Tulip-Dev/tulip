/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

namespace tlp {

class GlGraphRenderingParameters {
%TypeHeaderCode
#include <tulip/GlGraphRenderingParameters.h>
%End

%Docstring
.. versionadded:: 4.4

That class holds all the rendering parameters for a Tulip OpenGL graph drawing.
Use :meth:`tulipgui.tlpgui.NodeLinkDiagramComponent.getRenderingParameters` to get an instance on a such object.
%End

public:

      GlGraphRenderingParameters();

      tlp::DataSet getParameters() const;
      void setParameters(const tlp::DataSet &);

      void setDisplayEdgesExtremities(const bool state);
      bool displayEdgesExtremities()const;

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

      void setElementsOrderingProperty(tlp::NumericProperty* property);
      tlp::NumericProperty* elementsOrderingProperty() const;

      void setElementsZOrdered(const bool state);
      bool elementsZOrdered() const;

      void setEdges3D(const bool state);
      bool edges3D() const;

      void setMaxEdgesSizesToNodesSizes(const bool b);
      bool maxEdgesSizesToNodesSizes() const;

      void setSelectionColor(const tlp::Color &color);
      tlp::Color selectionColor() const;

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

      void setDisplayFilteringProperty(tlp::BooleanProperty* filteringProperty);
      tlp::BooleanProperty* displayFilteringProperty() const;

      void setBypassLodSystem(bool bypassLodSystem);
      bool bypassLodSystem() const;
};
};

