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


/** \brief Class used by GlGraphComposite to display the graph
 *
 * This class is used by GlGraphComposite to display into OpenGL the graph contained by the GlGraphComposite
 */
class TLP_GL_SCOPE GlGraphRenderer {

public:

  /**
   * Constructor
   * \param inputData : GlGraphInputData used by renderer to display the graph
   * \param parameters : GlGraphRenderingParameters used by renderer to display the graph
   */
  GlGraphRenderer(const GlGraphInputData *inputData):inputData(inputData),graphModified(true),selectionDrawActivate(false),selectionIdMap(NULL),selectionCurrentId(NULL) {
  }

  virtual ~GlGraphRenderer() {}

  /**
   * This function is call by GlGraphComposite to draw the graph
   * \param lod : lod used to this Rendering
   * \param camera : camera used to this rendering
   */
  virtual void draw(float lod,Camera* camera) = 0;

  /**
   * This function is call by GlGraphComposite to inform renderer that the next rendering will be for a selection
   * You have the type of the selection and after you have to use and increment currentId, for example you can have this function call with 4 into currentId,
   * if you graph contains 2 nodes you have to create 2 SelectedEntity object and add these object in the map : idMap.
   * You will have a code like this :
   *   idMap[currentId]=SelectedEntity(graph,node1.id,NODE_SELECTED);
   *   currentId++;
   *   idMap[currentId]=SelectedEntity(graph,node2.id,NODE_SELECTED);
   *   currentId++;
   *
   * And when you do the rendering (in draw function) of these nodes you have to call glLoadName(...) function
   *
   * \param type : type of selection (RenderingSimpleEntities,RenderingNodes,RenderingEdges,RenderingAll,RenderingWithoutRemove)
   * \param idMap : in this map you have to store SelectedEntity object
   */
  virtual void initSelectionRendering(RenderingEntitiesFlag type,std::map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId) = 0;

  /**
   * You can use this funtion if you want to inject a visitor on the graph
   */
  virtual void visitGraph(GlSceneVisitor *visitor,bool visitHiddenEntities=false);

  /**
   * This function set if the content of the graph is modified
   */
  void setGraphModified(bool graphModified) {
    this->graphModified=graphModified;
  }

protected:

  void visitNodes(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities=false);
  void visitEdges(Graph *graph,GlSceneVisitor *visitor,bool visitHiddenEntities=false);

  const GlGraphInputData* inputData;

  bool graphModified;

  bool selectionDrawActivate;
  RenderingEntitiesFlag selectionType;
  std::map<unsigned int, SelectedEntity> *selectionIdMap;
  unsigned int *selectionCurrentId;
};
}

#endif
