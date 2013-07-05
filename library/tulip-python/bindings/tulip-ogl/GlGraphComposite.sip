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

};
};

