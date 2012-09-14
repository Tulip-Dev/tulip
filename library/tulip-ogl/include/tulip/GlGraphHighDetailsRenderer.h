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
#ifndef Tulip_GLGRAPHHIGHDETAILSRENDERER_H
#define Tulip_GLGRAPHHIGHDETAILSRENDERER_H

#include <tulip/GlGraphRenderer.h>

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


/** \brief Class to display graph with old rendering engine
 *
 * This class display graph with :
 *  - Nodes and edges shapes
 *  - LOD system
 * With this renderer you can perform selection
 *
 * See GlGraphRenderer documentation for functions documentations
 */
class TLP_GL_SCOPE GlGraphHighDetailsRenderer : public GlGraphRenderer {

public:

  GlGraphHighDetailsRenderer(const GlGraphInputData *inputData);
  GlGraphHighDetailsRenderer(const GlGraphInputData *inputData,GlScene *scene);

  ~GlGraphHighDetailsRenderer();

  virtual void draw(float lod,Camera* camera);

  virtual void selectEntities(Camera *camera,RenderingEntitiesFlag type,int , int , int , int , std::vector<SelectedEntity>& selectedEntities);

protected:

  void initSelectionRendering(RenderingEntitiesFlag type,std::map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId);

  void buildSortedList();

  void drawLabelsForComplexEntities(bool drawSelected,OcclusionTest *occlusionTest,LayerLODUnit &layerLODUnit);

  GlLODCalculator *lodCalculator;


  GlScene *baseScene;
  GlScene *fakeScene;
};
}

#endif
