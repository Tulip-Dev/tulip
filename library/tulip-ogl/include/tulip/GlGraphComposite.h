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

struct ltnode
{
  bool operator()(GlNode n1,GlNode n2) const
  {
    return n1.id<n2.id;
  }
};

struct ltedge
{
  bool operator()(GlEdge e1, GlEdge e2) const
  {
    return e1.id<e2.id;
  }
};


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
    ~GlGraphComposite();

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
    virtual void acceptVisitor(GlSceneVisitor *visitor){
      addNodes();

      if(isDisplayEdges() || parameters.isViewEdgeLabel()) {
        for(std::set<GlEdge>::iterator it=edges.begin();it!=edges.end();++it) {
          GlEdge *tmp=(GlEdge*)(&(*it));
          tmp->acceptVisitor(visitor);
        }
      }

      if(isDisplayNodes()) {
        for(std::set<GlNode>::iterator it=nodes.begin();it!=nodes.end();++it) {
          GlNode *tmp=(GlNode*)(&(*it));
          tmp->acceptVisitor(visitor);
        }
      }

      if(isDisplayMetaNodes()) {
        for(std::set<GlMetaNode>::iterator it=metaNodes.begin();it!=metaNodes.end();++it) {
          GlMetaNode *tmp=(GlMetaNode*)(&(*it));
          tmp->acceptVisitor(visitor);
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

    /**
     * Get the data in XML form
     */
    void getXML(xmlNodePtr rootNode);

    /**
     * Set the data with XML
     */
    void setWithXML(xmlNodePtr rootNode);

    //Delegate
    void setDisplayNodes(bool display) {parameters.setDisplayNodes(display);}
    void setDisplayMetaNodes(bool display) {parameters.setDisplayMetaNodes(display);}
    void setDisplayEdges(bool display) {parameters.setDisplayEdges(display);}
    void setDisplayNodesLabel(bool display) {parameters.setViewNodeLabel(display);}
    void setDisplayMetaNodesLabel(bool display) {parameters.setViewMetaLabel(display);}
    void setDisplayEdgesLabel(bool display) {parameters.setViewEdgeLabel(display);}
    void setSelectedNodesStencil(int stencil) {parameters.setSelectedNodesStencil(stencil);}
    void setSelectedMetaNodesStencil(int stencil) {parameters.setSelectedMetaNodesStencil(stencil);}
    void setSelectedEdgesStencil(int stencil) {parameters.setSelectedEdgesStencil(stencil);}
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
    int getSelectedNodesStencil() {return parameters.getSelectedNodesStencil();}
    int getSelectedMetaNodesStencil() {return parameters.getSelectedMetaNodesStencil();}
    int getSelectedEdgesStencil() {return parameters.getSelectedEdgesStencil();}
    int getNodesStencil() {return parameters.getNodesStencil();}
    int getMetaNodesStencil() {return parameters.getMetaNodesStencil();}
    int getEdgesStencil() {return parameters.getEdgesStencil();}
    int getNodesLabelStencil() {return parameters.getNodesLabelStencil();}
    int getMetaNodesLabelStencil() {return parameters.getMetaNodesLabelStencil();}
    int getEdgesLabelStencil() {return parameters.getEdgesLabelStencil();}

  protected:

    GlGraphRenderingParameters parameters;
    GlGraphInputData inputData;

    std::set<GlNode,ltnode> nodes;
    std::set<GlMetaNode,ltnode> metaNodes;
    std::set<GlEdge,ltedge> edges;

    std::vector<unsigned int> nodesToAdd;

  };
}

#endif
