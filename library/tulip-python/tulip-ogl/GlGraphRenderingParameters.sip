namespace tlp {

/**
 * That class defines all the paremters used by the GlGraph class
 * during the rendering of a graph.
 */
class GlGraphRenderingParameters {
%TypeHeaderCode
#include <tulip/GlGraphRenderingParameters.h>
%End

public:

  GlGraphRenderingParameters();

  tlp::DataSet getParameters() const;

  void setParameters(const tlp::DataSet &);

  /**
   * @brief Gets the LOD below which the point mode is activated.
   *
   * @return float
   **/
  float getPointModeLOD() const;

  /**
   * @brief Sets the LOD below which the point mode is activated.
   * If the lod is strictly less than the value, the point mode will be activated.
   *
   * @param pointModeLOD The LOD at which to enable point mode.
   * @return void
   **/
  void setPointModeLOD(const float pointModeLOD);

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
   * @brief Defines which property is used for the element ordering.
   *
   * @param property The property to use to order the elements.
   * @return void
   **/
  void setElementOrderingProperty(tlp::DoubleProperty* property);

  /**
   * @brief Gets the property used for element ordering. Setting it to NULL will use viewMetric if the ordering is enabled.
   *
   * @return :DoubleProperty* The Property curretly used.
   **/
  tlp::DoubleProperty* getElementOrderingProperty() const;

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
   * \deprecated Use getLabelsDensity instead
   */
  unsigned int getLabelsBorder() const;

  /**
   * Get the size of the border that is added to any label when
   * using bitmap fonts. Setting this parameter to a high value
   * enables de reduce the label density in the final drawing.
   * Default value is set to 2 which enables readable labels.
   * \deprecated Use setLabelsDensity instead
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
  void setSelectionColor(const tlp::Color &color);

  /**
   * return the selection color
   */
  tlp::Color getSelectionColor();

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
   * \deprecated Use getLabelsDensity instead
   */
  bool isLabelOverlaped();

  /**
   * Set if labels can be overlaped
   * \deprecated Use setLabelsDensity instead
   */
  void setLabelOverlaped(bool state);

  /**
   * Return labels density
   * This density must be in interval -100 100
   * -100 : No labels
   * 0    : No overlap
   * 100  : All labels
   */
  int getLabelsDensity();

  /**
   * Set labels density
   * This density must be in interval -100 100
   * -100 : No labels
   * 0    : No overlap
   * 100  : All labels
   */
  void setLabelsDensity(int density);

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
   * return if labels are billboarded
   */
  bool getLabelsAreBillboarded();

  /**
   * Set if labels are billboarded
   */
  void setLabelsAreBillboarded(bool billboarded);

  /**
   * return if polylines are bicolored
   */
  bool getPolylines3DAreBicolored();

  /**
   * Set if polylines are bicolored
   */
  void setPolylines3DAreBicolored(bool bicolored);

  /**
   * This property is use to filter nodes/edges display, for a node/edge if this property is false : the node/edge will not be diplayed
   * Warning : this property will not be saved in DataSet
   */
  void setDisplayFilteringProperty(tlp::BooleanProperty *filteringProperty);

  /**
   * Get the boolean property use to filtering displaying of nodes/edges
   */
  tlp::BooleanProperty * getDisplayFilteringProperty() const;

};
};

