namespace tlp {

/** \brief Class use to represent a graph
 *
 * GlComposite use to represent a graph with nodes, metanodes and edges
 */
class GlGraphComposite : tlp::GlComposite /NoDefaultCtors/ {
%TypeHeaderCode
#include <tulip/GlGraphComposite.h>
%End

public:

  /**
   * Build a GlGraphComposite with the graph data
   */
  GlGraphComposite(tlp::Graph* graph);

  ~GlGraphComposite();

  /**
   * Set the rendering parameters use for rendering
   */
  void setRenderingParameters(const tlp::GlGraphRenderingParameters &parameter);

  /**
   * Return a pointer on rendering parameters used for rendering
   */
  tlp::GlGraphRenderingParameters* getRenderingParameters();
%MethodCode
    sipRes = new tlp::GlGraphRenderingParameters(sipCpp->getRenderingParameters());
%End

  /**
   * Return the inputData use by the composite
   */
  tlp::GlGraphInputData* getInputData();

  /**
   * Function used to visit composite's children
   */
  virtual void acceptVisitor(tlp::GlSceneVisitor *visitor);

  /**
   * Return set of metaNodes
   */
  std::set<tlp::node> &getMetaNodes();

  /**
   * Get the data in XML form
   */
  void getXML(_xmlNode* rootNode);

  /**
   * Set the data with XML
   */
  void setWithXML(_xmlNode* rootNode);

  //Delegate
  void setDisplayNodes(bool display);

  void setDisplayMetaNodes(bool display);

  void setDisplayEdges(bool display);

  void setDisplayNodesLabel(bool display);

  void setDisplayMetaNodesLabel(bool display);

  void setDisplayEdgesLabel(bool display);

  void setSelectedNodesStencil(int stencil);

  void setSelectedMetaNodesStencil(int stencil);

  void setSelectedEdgesStencil(int stencil);

  void setNodesStencil(int stencil);

  void setMetaNodesStencil(int stencil);

  void setEdgesStencil(int stencil);

  void setNodesLabelStencil(int stencil);

  void setMetaNodesLabelStencil(int stencil);

  void setEdgesLabelStencil(int stencil);

  bool isDisplayNodes();

  bool isDisplayMetaNodes();

  bool isDisplayEdges();

  bool isDisplayNodesLabel();

  bool isDisplayMetaNodesLabel();

  bool isDisplayEdgesLabel();

  int getSelectedNodesStencil();

  int getSelectedMetaNodesStencil();

  int getSelectedEdgesStencil();

  int getNodesStencil();

  int getMetaNodesStencil();

  int getEdgesStencil();

  int getNodesLabelStencil();

  int getMetaNodesLabelStencil();

  int getEdgesLabelStencil();

};
};

