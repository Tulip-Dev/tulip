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

#ifndef Tulip_GLGRAPHCOMPOSITE_H
#define Tulip_GLGRAPHCOMPOSITE_H

#include <tulip/GlComposite.h>
#include <tulip/Observable.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlScene.h>

namespace tlp {

class Graph;
class GlGraphRenderer;

/**
 * @ingroup OpenGL
 * @brief Class use to visualize graph in OpenGL Tulip engine
 *
 * GlSimpleEntity specialisation used to visualize graph in GlScene system
 * @see GlSimpleEntity
 * @see GlScene
 *
 * To visualize graph you have to create a new GlGraphComposite and add it to a GlLayer of a GlScene
 * After that you can change some visualize parameters throw GlGraphRenderingParameters class
 * @see GlGraphRenderingParameters
 * @see getRenderingParametersPointer()
 *
 * To render the graph in OpenGL, GlGraphComposite use a GlGraphRenderer. So if you want to change the system to render the graph, you have to create a new GlGraphRender
 * @see GlGraphRenderer
 */
class TLP_GL_SCOPE GlGraphComposite : public GlComposite, public Observable {

public:

  /**
   * @brief Build a GlGraphComposite with the graph data
   *
   * You can specify a GlGraphRenderer, if you don't do this a GlGraphHighDetailsRenderer will be used to display the graph
   */
  GlGraphComposite(Graph* graph, GlGraphRenderer *graphRenderer=NULL);

  /**
   * @brief Build a GlGraphComposite with the graph data
   *
   * Is better to use the other one constructor
   *
   * This graph composite is associated to the scene passed in parameter
   */
  GlGraphComposite(Graph* graph, GlScene *scene);

  /**
   * @brief Destructor
   */
  ~GlGraphComposite();

  /**
   * @brief Return a copy of rendering parameters use for rendering
   *
   * So after you have to call setRenderingParameters
   */
  const GlGraphRenderingParameters& getRenderingParameters();
  /**
   * @brief Set the rendering parameters use for rendering
   */
  void setRenderingParameters(const GlGraphRenderingParameters &parameter);

  /**
   * @brief Return a pointer on rendering parameters used for rendering
   *
   * With this function you don't have to call setRenderingParameters() function
   */
  GlGraphRenderingParameters* getRenderingParametersPointer();

  /**
   * @brief Return the inputData use by the composite
   *
   * In GlGraphInputData you have properties used to render the graph
   */
  GlGraphInputData* getInputData();

  /**
   * @brief Return the graph used by this GlGraphComposite
   */
  Graph *getGraph() {
    return inputData.getGraph();
  }

///@cond DOXYGEN_HIDDEN

  /**
   * Function used to visit composite's children
   */
  virtual void acceptVisitor(GlSceneVisitor *visitor);
  /**
   * You have to use this function if you want to visit nodes/edges of the graph composite
   */
  virtual void acceptVisitorOnGraph(GlSceneVisitor *visitor);

  virtual void draw(float lod,Camera* camera);

  virtual void selectEntities(Camera *camera,RenderingEntitiesFlag type,int x, int y, int w, int h, std::vector<SelectedEntity>& selectedEntities);

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

  GlGraphRenderer *getRenderer() {
    return graphRenderer;
  }

  /**
   * @brief setRenderer Delete the old renderer and replace it by the new one. If the new renderer is equal to NULL create a GlGraphHighDetailsRenderer.
   */
  void setRenderer(tlp::GlGraphRenderer*);

///@endcond

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

protected:

///@cond DOXYGEN_HIDDEN

  void treatEvent(const Event& evt);

///@endcond

  GlGraphRenderingParameters parameters;
  GlGraphInputData inputData;
  Graph *rootGraph;

  GlGraphRenderer *graphRenderer;

  bool nodesModified;
  std::set<node> metaNodes;

};
}

#endif
