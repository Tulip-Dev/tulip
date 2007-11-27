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
      for(std::vector<GlNode>::iterator it=nodes.begin();it!=nodes.end();++it) {
	(*it).acceptVisitor(visitor);
      }
      for(std::vector<GlEdge>::iterator it=edges.begin();it!=edges.end();++it) {
	if(parameters.isDisplayEdges() || parameters.isViewEdgeLabel())
	  (*it).acceptVisitor(visitor);
      }
      for(std::vector<GlMetaNode>::iterator it=metaNodes.begin();it!=metaNodes.end();++it) {
	(*it).acceptVisitor(visitor);
      }
    }

    virtual void addNode(Graph *,const node );
    virtual void addEdge(Graph *,const edge );
    virtual void delNode(Graph *,const node );
    virtual void delEdge(Graph *,const edge );
    virtual void destroy(Graph *);

  protected:

    GlGraphRenderingParameters parameters;
    GlGraphInputData inputData;

    std::vector<GlNode> nodes;
    std::vector<GlMetaNode> metaNodes;
    std::vector<GlEdge> edges;

  };
}

#endif
