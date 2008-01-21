#ifndef Tulip_GLGRAPHCOMPOSITE_H
#define Tulip_GLGRAPHCOMPOSITE_H

#include "tulip/GlComposite.h"

#include <vector>

#include <tulip/Graph.h>
#include <tulip/ObservableGraph.h>

#include "tulip/GlGraphRenderingParameters.h"
#include "tulip/GlGraphInputData.h"
#include "tulip/GlNode.h"
#include "tulip/GlEdge.h"
#include "tulip/GlMetaNode.h"

namespace tlp {

  /** \brief Class use to represent a graph 
   * 
   * GlComposite use to represent a graph with nodes, metanodes and edges
   */
  class TLP_GL_SCOPE GlGraphComposite : public GlComposite, public GraphObserver {

  
  public:

    /**
     * Build a GlGraphComposite with the graph data
     */ 
    GlGraphComposite(Graph* graph);
    ~GlGraphComposite() {inputData.graph->removeObserver(this);}

    /**
     * Build list of Nodes, Edges and MetaNodes
     */
    void buildLists();
    /**
     * Return the rendering parameters use for rendering
     */
    const GlGraphRenderingParameters& getRenderingParameters();
    /**
     * Set the rendering parameters use for rendering
     */
    void setRenderingParameters(const GlGraphRenderingParameters &parameter);

    /**
     * Return the inputData use by the composite
     */
    GlGraphInputData* getInputData();

    /**
     * Function used to visit composite's children
     */
    virtual void acceptVisitor(GlSceneVisitor *visitor) {
      addNodes();
      if(isDisplayNodes()) {
	for(std::vector<GlNode>::iterator it=nodes.begin();it!=nodes.end();++it) {
	  (*it).acceptVisitor(visitor);
	}
      }
      
      if(isDisplayEdges()) {
	for(std::vector<GlEdge>::iterator it=edges.begin();it!=edges.end();++it) {
	  if(parameters.isDisplayEdges() || parameters.isViewEdgeLabel())
	    (*it).acceptVisitor(visitor);
	}
      }
      
      if(isDisplayMetaNodes()) {
	for(std::vector<GlMetaNode>::iterator it=metaNodes.begin();it!=metaNodes.end();++it) {
	  (*it).acceptVisitor(visitor);
	}
      }

    }

    /**
     * Function use by the GraphObserver when a node is create in the graph
     */
    virtual void addNode(Graph *,const node );
    /**
     * Function use by the GraphObserver when an edge is create in the graph
     */
    virtual void addEdge(Graph *,const edge );
    /**
     * Function use by the GraphObserver when a node is delete in the graph
     */
    virtual void delNode(Graph *,const node );
    /**
     * Function use by the GraphObserver when an edge is delete in the graph
     */
    virtual void delEdge(Graph *,const edge );
    /**
     * Function use by the GraphObserver when the graph is delete
     */
    virtual void destroy(Graph *);

    /**
     * Function used to add new nodes
     */
    void addNodes();

    //Delegate 
    void setDisplayNodes(bool display) {parameters.setDisplayNodes(display);}
    void setDisplayMetaNodes(bool display) {parameters.setDisplayMetaNodes(display);}
    void setDisplayEdges(bool display) {parameters.setDisplayEdges(display);}
    void setDisplayNodesLabel(bool display) {parameters.setViewNodeLabel(display);}
    void setDisplayMetaNodesLabel(bool display) {parameters.setViewMetaLabel(display);}
    void setDisplayEdgesLabel(bool display) {parameters.setViewEdgeLabel(display);}
    void setNodesStencil(int stencil) {parameters.setNodesStencil(stencil);}
    void setMetaNodesStencil(int stencil) {parameters.setMetaNodesStencil(stencil);}
    void setEdgesStencil(int stencil) {parameters.setEdgesStencil(stencil);}
    void setNodesLabelStencil(int stencil) {parameters.setNodesLabelStencil(stencil);}
    void setMetaNodesLabelStencil(int stencil) {parameters.setMetaNodesLabelStencil(stencil);}
    void setEdgesLabelStencil(int stencil) {parameters.setEdgesLabelStencil(stencil);}

    bool isDisplayNodes() {return parameters.isDisplayNodes();}
    bool isDisplayMetaNodes() {return parameters.isDisplayMetaNodes();}
    bool isDisplayEdges() {return parameters.isDisplayEdges();}
    bool isDisplayNodesLabel() {return parameters.isViewNodeLabel();}
    bool isDisplayMetaNodesLabel() {return parameters.isViewMetaLabel();}
    bool isDisplayEdgesLabel() {return parameters.isViewEdgeLabel();}
    int getNodesStencil() {return parameters.getNodesStencil();}
    int getMetaNodesStencil() {return parameters.getMetaNodesStencil();}
    int getEdgesStencil() {return parameters.getEdgesStencil();}
    int getNodesLabelStencil() {return parameters.getNodesLabelStencil();}
    int getMetaNodesLabelStencil() {return parameters.getMetaNodesLabelStencil();}
    int getEdgesLabelStencil() {return parameters.getEdgesLabelStencil();}

  protected:

    GlGraphRenderingParameters parameters;
    GlGraphInputData inputData;

    std::vector<GlNode> nodes;
    std::vector<GlMetaNode> metaNodes;
    std::vector<GlEdge> edges;

    std::vector<unsigned int> nodesToAdd;

  };
}

#endif
