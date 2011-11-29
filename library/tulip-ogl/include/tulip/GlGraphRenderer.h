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
#ifndef Tulip_GLGRAPHRENDERER_H
#define Tulip_GLGRAPHRENDERER_H

#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlScene.h>

namespace tlp {

class Graph;

class TLP_GL_SCOPE GlGraphRenderer {

public:

  /**
   * Build a GlGraphComposite with the graph data
   */
  GlGraphRenderer(Graph* graph,GlGraphInputData &inputData,GlGraphRenderingParameters &parameters):rootGraph(graph->getRoot()),inputData(inputData),parameters(parameters),haveToSort(true),selectionDrawActivate(false),selectionIdMap(NULL),selectionCurrentId(NULL){
  }

  virtual ~GlGraphRenderer(){}

  virtual void draw(float lod,Camera* camera) = 0;

  virtual void initSelectionRendering(RenderingEntitiesFlag type,std::map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId) = 0;

  virtual void visitGraph(GlSceneVisitor *visitor,bool visitHiddenEntities=false) = 0;

  void setHaveToSort(bool haveToSort){
    this->haveToSort=haveToSort;
  }

protected:

  Graph *rootGraph;
  const GlGraphInputData &inputData;
  const GlGraphRenderingParameters &parameters;

  bool haveToSort;

  bool selectionDrawActivate;
  RenderingEntitiesFlag selectionType;
  std::map<unsigned int, SelectedEntity> *selectionIdMap;
  unsigned int *selectionCurrentId;
};
}

#endif
