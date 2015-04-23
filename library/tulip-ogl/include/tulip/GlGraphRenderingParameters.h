/*
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

#ifndef Tulip_GLGRAPHRENDERINGPARAMETERS_H
#define Tulip_GLGRAPHRENDERINGPARAMETERS_H

#include <tulip/Color.h>
#include <tulip/DataSet.h>
#include <tulip/tulipconf.h>

namespace tlp {

class NumericProperty;
class BooleanProperty;

/**
 * @ingroup OpenGL
 * @brief That class defines all the parameters used by GlGraphComposite to render a graph
 *
 * @see GlGraphComposite
 */
class TLP_GL_SCOPE GlGraphRenderingParameters {

public:
  /**
   * @brief Constructor
   */
  GlGraphRenderingParameters();

  /**
   * @brief Get parameters into a DataSet structure
   */
  DataSet getParameters() const;

  /**
   * @brief Set parameters with a DataSet structure
   */
  void setParameters(const DataSet &);

  /**
   * @brief activate or deactivate anlialiasing
   */
  void setAntialiasing(const bool state);

  /**
   * @brief Return true if antialiasing is activated else false.
   */
  bool isAntialiased()const;

  /**
   * @brief Activate or deactivate displaying of arrows along the graph's edges.
   */
  void setViewArrow(const bool state);

  /**
   * @brief Return true if arrows drawing is activated else false.
   */
  bool isViewArrow()const;

  /**
   * @brief Activate or deactivate displaying of labels on nodes.
   */
  void setViewNodeLabel(const bool state);

  /**
   * @brief Return true if drawing of nodes' labels is activated.
   */
  bool isViewNodeLabel() const;
  /**
   * @brief Activate or deactivate displaying of labels on nodes.
   */
  void setViewEdgeLabel(const bool state);

  /**
   * @brief Return true if drawing of nodes' labels is activated.
   */
  bool isViewEdgeLabel() const;

  /**
   * @brief Activate or deactivate displaying of labels outside the screen.
   */
  void setViewOutScreenLabel(const bool state);

  /**
   * @brief Return true if drawing of outscreen labels is activated.
   */
  bool isViewOutScreenLabel() const;

  /**
   * @brief Activate or deactivate interpolation of nodes colors along edge
   */
  void setEdgeColorInterpolate(const bool state);

  /**
   * @brief Return true if color interpolation is activated
   */
  bool isEdgeColorInterpolate() const;

  /**
   * @brief Activate or deactivate interpolation of nodes colors along edge
   */
  void setEdgeSizeInterpolate(const bool state);

  /**
   * @brief Return true if color interpolation is activated
   */
  bool isEdgeSizeInterpolate() const;

  /**
   * @brief Return the type of fonts used in the renderer(0=3D,1=bitmap,2=texture)
   */
  unsigned int getFontsType() const;

  /**
   * @brief Set fonts type used in the renderer (0=3D,1=bitmap,2=texture)
   */
  void setFontsType(unsigned int type);

  /**
   * @brief Set the path for the texture loading
   */
  void setTexturePath(const std::string& path);

  /**
   * @brief Return the absolute path used for texture loading
   */
  std::string getTexturePath() const;

  /**
   * @brief Set the absolute path used for the fonts loading
   */
  void setFontsPath(const std::string& path);

  /**
   * @brief Return the absolute path used for the fonts loading
   */
  std::string getFontsPath() const;

  /**
   * @brief If true and if labels are activated label inside meta nodes will be displayed
   */
  void setViewMetaLabel(const bool state);

  /**
   * @brief Return the true if viewMetaLabels is enable else false
   */
  bool isViewMetaLabel() const;

  /**
   * @brief If true edges are displayed.
   */
  void setDisplayEdges(const bool state);

  /**
   * @brief Return true if edges are displayed.
   */
  bool isDisplayEdges() const;

  /**
   * @brief If true nodes are displayed.
   */
  void setDisplayNodes(const bool state);

  /**
   * @brief Return true if nodes are displayed.
   */
  bool isDisplayNodes() const;

  /**
   * @brief If true meta-nodes are displayed.
   */
  void setDisplayMetaNodes(const bool state);

  /**
   * @brief Return true if meta-nodes are displayed.
   */
  bool isDisplayMetaNodes() const;

  /**
   * @brief set stencil number for selected nodes
   *
   * Stencil is an OpenGl system to ensure that other entity can't be displayed above this entity. It's a "guaranted visibility" system.
   * A small number causes a guaranted visibility
   * Default value in Tulip is 0xFFFF (greater integer)
   * And when we have stencil on entity value is 0x2
   */
  void setSelectedNodesStencil(const int stencil);

  /**
   * @brief get stencil number for selected nodes
   */
  int getSelectedNodesStencil() const;

  /**
   * @brief set stencil number for selected meta-nodes
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setSelectedMetaNodesStencil(const int stencil);

  /**
   * @brief get stencil number for selected meta-nodes
   */
  int getSelectedMetaNodesStencil() const;

  /**
   * @brief set stencil number for selected edges
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setSelectedEdgesStencil(const int stencil);

  /**
   * @brief get stencil number for selected edges
   */
  int getSelectedEdgesStencil() const;

  /**
   * @brief set stencil number for nodes
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setNodesStencil(const int stencil);

  /**
   * @brief get stencil number for nodes
   */
  int getNodesStencil() const;

  /**
   * @brief set stencil number for meta-nodes
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setMetaNodesStencil(const int stencil);

  /**
   * @brief get stencil number for meta-nodes
   */
  int getMetaNodesStencil() const;

  /**
   * @brief set stencil number for edges
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setEdgesStencil(const int stencil);

  /**
   * @brief get stencil number for edges
   */
  int getEdgesStencil() const;

  /**
   * @brief set stencil number for nodes label
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setNodesLabelStencil(const int stencil);

  /**
   * @brief get stencil number for nodes label
   */
  int getNodesLabelStencil() const;

  /**
   * @brief set stencil number for meta-nodes label
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setMetaNodesLabelStencil(const int stencil);

  /**
   * @brief get stencil number for meta-nodes label
   */
  int getMetaNodesLabelStencil() const;

  /**
   * @brief set stencil number for edges label
   *
   * See setSelectedNodesStencil() documentation for more details on stencil number
   */
  void setEdgesLabelStencil(const int stencil);

  /**
   * @brief get stencil number for edges label
   */
  int getEdgesLabelStencil() const;

  /**
   * @brief If true node and edges are ordered according to the viewMetric property
   */
  void setElementOrdered(const bool state);

  /**
   * @brief return true if element ordering is activated
   */
  bool isElementOrdered() const;

  /**
   * @brief Defines which property is used for the element ordering.
   *
   * @param property The property to use to order the elements.
   * @return void
   **/
  void setElementOrderingProperty(tlp::NumericProperty* property);

  /**
   * @brief Gets the property used for element ordering. Setting it to NULL will use viewMetric if the ordering is enabled.
   *
   * @return :NumericProperty* The Property currently used.
   **/
  tlp::NumericProperty* getElementOrderingProperty() const;

  /**
   * @brief If true node and edges are ordered according to the distance with camera
   */
  void setElementZOrdered(const bool state);

  /**
   * @brief return true if element Z ordering is activated
   */
  bool isElementZOrdered() const;

  /**
   * @brief if true use 3D displaying of edges instead of 2D lines on ALL edges
   */
  void setEdge3D(const bool state);

  /**
   * @brief return true if 3D drawing of edges is activated
   */
  bool isEdge3D() const;

  /**
   * @brief Get the size of the border that is added to any label when using bitmap fonts.
   *
   * @deprecated Use getLabelsDensity instead
   */
  unsigned int _DEPRECATED getLabelsBorder() const;

  /**
   * @brief Get the size of label border
   *
   * Get the size of the border that is added to any label when
   * using bitmap fonts. Setting this parameter to a high value
   * enables de reduce the label density in the final drawing.
   * Default value is set to 2 which enables readable labels.
   * @deprecated Use setLabelsDensity instead
   */
  void _DEPRECATED setLabelsBorder(const unsigned int);

  /**
   * @brief set if Edge max size is block to node size
   */
  void setEdgesMaxSizeToNodesSize(const bool b);

  /**
   * @brief return if Edge max size is block to node size
   */
  bool getEdgesMaxSizeToNodesSize() const;

  /**
   * @brief if true draw graph with feedback information (very low)
   */
  void setFeedbackRender(bool feedback);

  /**
   * @brief return true if the graph is draw with feedback information
   */
  bool getFeedbackRender() const;

  /**
   * @brief Change the selection color
   */
  void setSelectionColor(const Color &color);

  /**
   * @brief return the selection color
   */
  Color getSelectionColor() const;

  /**
   * @brief return if labels are scaled to node size
   */
  bool isLabelScaled() const;

  /**
   * Set if labels are scaled to node size
   */
  void setLabelScaled(bool state);

  /**
   * @brief return if labels are rendered with a fixed font size (dynamic size otherwise)
   */
  bool isLabelFixedFontSize() const;

  /**
   * Set if labels must be rendered with a fixed font size (dynamic size otherwise)
   */
  void setLabelFixedFontSize(bool state);

  /**
   * @brief Return labels -density
   *
   * This density must be in interval -100 100
   * -100 : No labels
   * 0    : No overlap
   * 100  : All labels
   */
  int getLabelsDensity() const;

  /**
   * @brief Set labels density
   *
   * This density must be in interval -100 100
   * -100 : No labels
   * 0    : No overlap
   * 100  : All labels
   */
  void setLabelsDensity(int density);

  /**
   * @brief return min size of label : this min size is only used if labels are not scaled and are not rendered with a fixed font size
   */
  int getMinSizeOfLabel() const;

  /**
   * @brief Set min size of label : this min size is only used if labels are not scaled and are not rendered with a fixed font size
   */
  void setMinSizeOfLabel(int size);

  /**
   * @brief return max size of label : this max size is only used if labels are not scaled and are not rendered with a fixed font size
   */
  int getMaxSizeOfLabel() const;

  /**
   * @brief Set max size of label : this max size is only used if labels are not scaled and are not rendered with a fixed font size
   */
  void setMaxSizeOfLabel(int size);

  /**
   * @brief return if labels are billboarded
   */
  bool getLabelsAreBillboarded() const {
    return _labelsAreBillboarded;
  }

  /**
   * @brief Set if labels are billboarded
   */
  void setLabelsAreBillboarded(bool billboarded) {
    _labelsAreBillboarded=billboarded;
  }

  /**
   * @brief This property is use to filter nodes/edges display, for a node/edge if this property is false : the node/edge will not be diplayed
   *
   * @Warning : this property will not be saved in DataSet
   */
  void setDisplayFilteringProperty(BooleanProperty *filteringProperty) {
    _displayFilteringProperty=filteringProperty;
  }

  /**
   * @brief Get the boolean property use to filtering displaying of nodes/edges
   */
  BooleanProperty * getDisplayFilteringProperty() const {
    return _displayFilteringProperty;
  }

private:

  bool _antialiased;
  bool _viewArrow;
  bool _viewNodeLabel;
  bool _viewEdgeLabel;
  bool _viewMetaLabel;
  bool _viewOutScreenLabel;
  bool _elementOrdered;
  bool _edgeColorInterpolate;
  bool _edge3D;
  bool _edgeSizeInterpolate;
  bool _displayEdges;
  bool _displayNodes;
  bool _displayMetaNodes;
  bool _elementZOrdered;
  int _selectedNodesStencil;
  int _selectedMetaNodesStencil;
  int _selectedEdgesStencil;
  int _nodesStencil;
  int _metaNodesStencil;
  int _edgesStencil;
  int _nodesLabelStencil;
  int _metaNodesLabelStencil;
  int _edgesLabelStencil;
  bool _labelScaled;
  bool _labelFixedFontSize;
  int _labelMinSize;
  int _labelMaxSize;
  int _labelsDensity;
  bool _labelsAreBillboarded;
  std::string _fontsPath;
  std::string _texturePath;
  bool _edgesMaxSizeToNodesSize;
  bool _feedbackRender;
  Color _selectionColor;
  BooleanProperty *_displayFilteringProperty;
  NumericProperty* _elementOrderingProperty;
};

/** \brief Class to get the default selection color
 */
class TLP_GL_SCOPE GlDefaultSelectionColorManager {
public:
  static void setManager(GlDefaultSelectionColorManager *);
  static tlp::Color getDefaultSelectionColor();
  virtual tlp::Color defaultSelectionColor()=0;

private:
  static GlDefaultSelectionColorManager* manager;
};
}

#endif // Tulip_GLGRAPH_H
