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
///@cond DOXYGEN_HIDDEN

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
   * This function is call by GlGraphComposite to selected entities into the graph
   * \param type : type of selected entities
   * \param x : x of the selected zone
   * \param y : y of the selected zone
   * \param w : width of the selected zone
   * \param h : height of the selected zone
   * \param selectedEntities : you have to put selected entities into this vector
   */
  virtual void selectEntities(Camera *camera,RenderingEntitiesFlag type,int x, int y, int w, int h, std::vector<SelectedEntity>& selectedEntities) = 0;

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
///@endcond