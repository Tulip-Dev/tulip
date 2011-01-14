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
#ifndef Tulip_GLGRAPHRENDERINGPARAMETERS_H
#define Tulip_GLGRAPHRENDERINGPARAMETERS_H

#include <string>
#include <tulip/Color.h>
#include <tulip/Reflect.h>
#include <tulip/DoubleProperty.h>

namespace tlp {

  /**
   * That class defines all the paremters used by the GlGraph class
   * during the rendering of a graph.
   */
  class TLP_GL_SCOPE GlGraphRenderingParameters {

  public:
    GlGraphRenderingParameters();
    //============================================
    DataSet getParameters() const;
    void    setParameters(const DataSet &);
    /**
     * activate or deactivate anlialiasing
     */
    void setAntialiasing(const bool state);
    /** return true if antialiasing is activated else false.
     */
    bool isAntialiased()const;
    /**
     * activate or deactivate displaying of arrows along the graph's edges.
     */
    void setViewArrow(const bool state);
    /** return true if arrows drawing is activated else false.
     * \sa setViewArrow
     */
    bool isViewArrow()const;
    /**
     * activate or deactivate displaying of labels on nodes.
     */
    void setViewNodeLabel(const bool state);
    /**
     * return true if drawing of nodes' labels is activated.
     */
    bool isViewNodeLabel() const;
    /**
     * activate or deactivate displaying of labels on nodes.
     */
    void setViewEdgeLabel(const bool state);
    /**
     * return true if drawing of nodes' labels is activated.
     */
    bool isViewEdgeLabel() const;
    /**
     * activate or deactivate displaying of labels outside the screen.
     */
    void setViewOutScreenLabel(const bool state);
    /**
     * return true if drawing of outscreen labels is activated.
     */
    bool isViewOutScreenLabel() const;
    /**
     *activate or deactivate interpolation of nodes colors along edge
     */
    void setEdgeColorInterpolate(const bool state);
    /**
     * return true if color interpolation is activated
     */
    bool isEdgeColorInterpolate() const;
    /**
     * activate or deactivate interpolation of nodes colors along edge
     */
    void setEdgeSizeInterpolate(const bool state);
    /**
     * return true if color interpolation is activated
     */
    bool isEdgeSizeInterpolate() const;
    /**
     * return the type of fonts used in the renderer(0=3D,1=bitmap,2=texture)
     */
    unsigned int getFontsType() const;
    /**
     * set fonts type used in the renderer (0=3D,1=bitmap,2=texture)
     */
    void setFontsType(unsigned int type);
    /**
     * set the path for the texture loading
     */
    void setTexturePath(const std::string& path);
    /**
     * return the absolute path used for texture loading
     */
    std::string getTexturePath() const;
    /**
     * set the absolute path used for the fonts loading
     */
    void setFontsPath(const std::string& path);
    /**
     * return the absolute path used for the fonts loading
     */
    std::string getFontsPath();
    /**
     * if true glgraph use the incremental rendering else display all elements in one pass
     */
    void setIncrementalRendering(const bool state);
    /**
     * return the incremental rendering state
     */
    bool isIncrementalRendering() const;
    /**
     * If true and if labels are activated label inside meta nodes will be displayed
     */
    void setViewMetaLabel(const bool state);
    /**
     * return the true if viewMetaLabels is enable else false
     */
    bool isViewMetaLabel() const;
    /**
     * If true edges are displayed.
     */
    void setDisplayEdges(const bool state);
    /**
     * return true if edges are displayed.
     */
    bool isDisplayEdges() const;
    /**
     * If true nodes are displayed.
     */
    void setDisplayNodes(const bool state);
    /**
     * return true if nodes are displayed.
     */
    bool isDisplayNodes() const;
    /**
     * If true meta-nodes are displayed.
     */
    void setDisplayMetaNodes(const bool state);
    /**
     * return true if meta-nodes are displayed.
     */
    bool isDisplayMetaNodes() const;
    /**
     * set stencil number for selected nodes
     */
    void setSelectedNodesStencil(const int stencil);
    /**
     * get stencil number for selected nodes
     */
    int getSelectedNodesStencil();
    /**
     * set stencil number for selected meta-nodes
     */
    void setSelectedMetaNodesStencil(const int stencil);
    /**
     * get stencil number for selected meta-nodes
     */
    int getSelectedMetaNodesStencil();
    /**
     * set stencil number for selected edges
     */
    void setSelectedEdgesStencil(const int stencil);
    /**
     * get stencil number for selected edges
     */
    int getSelectedEdgesStencil();
    /**
     * set stencil number for nodes
     */
    void setNodesStencil(const int stencil);
    /**
     * get stencil number for nodes
     */
    int getNodesStencil();
    /**
     * set stencil number for meta-nodes
     */
    void setMetaNodesStencil(const int stencil);
    /**
     * get stencil number for meta-nodes
     */
    int getMetaNodesStencil();
    /**
     * set stencil number for edges
     */
    void setEdgesStencil(const int stencil);
    /**
     * get stencil number for edges
     */
    int getEdgesStencil();
    /**
     * set stencil number for nodes label
     */
    void setNodesLabelStencil(const int stencil);
    /**
     * get stencil number for nodes label
     */
    int getNodesLabelStencil();
    /**
     * set stencil number for meta-nodes label
     */
    void setMetaNodesLabelStencil(const int stencil);
    /**
     * get stencil number for meta-nodes label
     */
    int getMetaNodesLabelStencil();
    /**
     * set stencil number for edges label
     */
    void setEdgesLabelStencil(const int stencil);
    /**
     * get stencil number for edges label
     */
    int getEdgesLabelStencil();
    /**
     * If true node and edges are ordered according to the viewMetric property
     */
    void setElementOrdered(const bool state);
    /**
     * return true if element ordering is activated
     */
    bool isElementOrdered() const;
    /**
     * If true node and edges are ordered according to the distance with camera
     */
    void setElementZOrdered(const bool state);
    /**
     * return true if element Z ordering is activated
     */
    bool isElementZOrdered() const;
    /**
     * if true use 3D displaying of edges instead of 2D lines on ALL edges
     */
    void setEdge3D(const bool state);
    /**
     * return true if 3D drawing of edges is activated
     */
    bool isEdge3D() const;
    /**
     * Get the size of the border that is added to any label when
     * using bitmap fonts.
     */
    unsigned int getLabelsBorder() const;
    /**
     * Get the size of the border that is added to any label when
     * using bitmap fonts. Setting this parameter to a high value
     * enables de reduce the label density in the final drawing.
     * Default value is set to 2 which enables readable labels.
     */
    void setLabelsBorder(const unsigned int);
    /**
     * set if Edge max size is block to node size
     */
    void setEdgesMaxSizeToNodesSize(const bool b);
    /**
     * return if Edge max size is block to node size
     */
    bool getEdgesMaxSizeToNodesSize() const;
    /**
     * if true draw graph with feedback information (very low)
     */
    void setFeedbackRender(bool feedback);
    /**
     * return true if the graph is draw with feedback information
     */
    bool getFeedbackRender();
    /**
     * Change the selection color
     */
    void setSelectionColor(const Color &color);
    /**
     * return the selection color
     */
    Color getSelectionColor();
    /**
     * return if labels are scaled to node size
     */
    bool isLabelScaled();
    /**
     * Set if labels are scaled to node size
     */
    void setLabelScaled(bool state);
    /**
     * return if labels can be overlaped
     */
    bool isLabelOverlaped();
    /**
     * Set if labels can be overlaped
     */
    void setLabelOverlaped(bool state);
    /**
     * return min size of label : this min size is only used if labels aren't be scaled
     */
    int getMinSizeOfLabel();
    /**
     * Set min size of label : this min size is only used if labels aren't be scaled
     */
    void setMinSizeOfLabel(int size);
    /**
     * return max size of label : this max size is only used if labels aren't be scaled
     */
    int getMaxSizeOfLabel();
    /**
     * Set max size of label : this max size is only used if labels aren't be scaled
     */
    void setMaxSizeOfLabel(int size);

		/**
		 * This property is use to filter nodes/edges display, for a node/edge if this property is false : the node/edge will not be diplayed
		 * Warning : this property will not be saved in DataSet
		 */
		void setDisplayFilteringProperty(BooleanProperty *filteringProperty) {
			_displayFilteringProperty=filteringProperty;
		}

		/**
		 * Get the boolean property use to filtering displaying of nodes/edges
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
    bool _incrementalRendering;
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
    bool _labelOverlaped;
    int _labelMinSize;
    int _labelMaxSize;
    unsigned int _labelsBorder;
    std::string _fontsPath;
    std::string _texturePath;
    bool _edgesMaxSizeToNodesSize;
    bool _feedbackRender;
    Color _selectionColor;
		BooleanProperty *_displayFilteringProperty;

  };
}

#endif // Tulip_GLGRAPH_H
