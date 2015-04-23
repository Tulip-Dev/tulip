/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/GlScene.h>
#include <tulip/tulipconf.h>

namespace tlp {

class Graph;
class GlGraphInputData;
class Camera;

/**
 * @ingroup OpenGL
 * @brief Class used by GlGraphComposite to render the graph in OpenGL
 *
 * To create a graph renderer, you have to implement two functions : draw() and selectEntities()
 * @see GlGraphComposite
 */
class TLP_GL_SCOPE GlGraphRenderer {

public:

  /**
   * @brief Constructor
   * \param inputData : GlGraphInputData used by renderer to display the graph (in input data you have pointers on properties used to render nodes/edges
   * \param parameters : GlGraphRenderingParameters used by renderer to display the graph
   */
  GlGraphRenderer(const GlGraphInputData *inputData);

  /**
   * @brief Destructor
   */
  virtual ~GlGraphRenderer() {}

  /**
   * @brief This function is call by GlGraphComposite to draw the graph
   *
   * If you reimplement this function you have to render nodes/edges. It's the most important function of GlGraphRenderer
   *
   * \param lod : lod used to this Rendering
   * \param camera : camera used to this rendering
   */
  virtual void draw(float lod,Camera* camera) = 0;

  /**
   * @brief This function is call by GlGraphComposite to selected entities into the graph
   * \param type : type of selected entities
   * \param x : x of the selected zone
   * \param y : y of the selected zone
   * \param w : width of the selected zone
   * \param h : height of the selected zone
   * \param selectedEntities : you have to put selected entities into this vector
   */
  virtual void selectEntities(Camera *camera,RenderingEntitiesFlag type,int x, int y, int w, int h, std::vector<SelectedEntity>& selectedEntities) = 0;

  /**
   * @brief You can use this funtion if you want to inject a visitor on the graph
   */
  virtual void visitGraph(GlSceneVisitor *visitor,bool visitHiddenEntities=false);

  /**
   * @brief This function set if the content of the graph is modified
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
