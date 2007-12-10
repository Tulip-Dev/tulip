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
  class TLP_GL_SCOPE GlGraphComposite : public GlComposite, public GraphObserver {

  
  public:

    GlGraphComposite(Graph* graph);
    ~GlGraphComposite() {inputData.graph->removeObserver(this);}

    /**
     * Return the rendering parameters used for rendering
     */
    const GlGraphRenderingParameters& getRenderingParameters();
    /**
     * Set the rendering parameters used for rendering
     */
    void setRenderingParameters(const GlGraphRenderingParameters &parameter);

    GlGraphInputData* getInputData();

    virtual void acceptVisitor(GlSceneVisitor *visitor) {
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

    virtual void addNode(Graph *,const node );
    virtual void addEdge(Graph *,const edge );
    virtual void delNode(Graph *,const node );
    virtual void delEdge(Graph *,const edge );
    virtual void destroy(Graph *);

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

  };
}

#endif
