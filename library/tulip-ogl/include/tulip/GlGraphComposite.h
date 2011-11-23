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
#ifndef Tulip_GLGRAPHCOMPOSITE_H
#define Tulip_GLGRAPHCOMPOSITE_H

#include <tulip/GlComposite.h>

#include <vector>

#include <tulip/Observable.h>

#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlNode.h>
#include <tulip/GlEdge.h>
#include <tulip/GlQuadTreeLODCalculator.h>
#include <tulip/GlScene.h>

namespace tlp {

class Graph;

/** \brief Class use to represent a graph
 *
 * GlComposite use to represent a graph with nodes, metanodes and edges
 */
class TLP_GL_SCOPE GlGraphComposite : public GlComposite, public Observable {

public:

  /**
   * Build a GlGraphComposite with the graph data
   */
  GlGraphComposite(Graph* graph);

  ~GlGraphComposite();

  /**
   * Return the rendering parameters use for rendering
   */
  const GlGraphRenderingParameters& getRenderingParameters();
  /**
   * Set the rendering parameters use for rendering
   */
  void setRenderingParameters(const GlGraphRenderingParameters &parameter);

  /**
   * Return a pointer on rendering parameters used for rendering
   */
  GlGraphRenderingParameters* getRenderingParametersPointer();

  /**
   * Return the inputData use by the composite
   */
  GlGraphInputData* getInputData();

  /**
   * Function used to visit composite's children
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor);

  virtual void draw(float lod,Camera* camera);

  void initSelectionRendering(RenderingEntitiesFlag type,std::map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId);

  /**
   * Return set of metaNodes
   */
  std::set<node> &getMetaNodes() {
    if(nodesModified) {
      metaNodes.clear();

      Graph *graph=inputData.getGraph();
      Iterator<node>* nodesIterator = graph->getNodes();

      while (nodesIterator->hasNext()) {
        node n=nodesIterator->next();

        if(graph->getNodeMetaInfo(n))
          metaNodes.insert(n);
      }

      delete nodesIterator;

      nodesModified=false;
    }

    return metaNodes;
  }

  /**
   * Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

protected:
  // override Observable::treatEvent
  void treatEvent(const Event& evt);

  void buildSortedList();
  void visitGraph(GlSceneVisitor *visitor,bool visitHiddenEntities=false);
  void visitNodes(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities=false);
  void visitEdges(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities=false);

  GlGraphRenderingParameters parameters;
  GlGraphInputData inputData;
  Graph *rootGraph;

  GlLODCalculator *lodCalculator;

  bool haveToSort;
  bool nodesModified;
  std::set<node> metaNodes;

  GlScene *fakeScene;
  bool selectionDrawActivate;
  RenderingEntitiesFlag selectionType;
  std::map<unsigned int, SelectedEntity> *selectionIdMap;
  unsigned int *selectionCurrentId;
};
}

#endif
