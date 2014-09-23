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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLGRAPHHIGHDETAILSRENDERER_H
#define Tulip_GLGRAPHHIGHDETAILSRENDERER_H

#include <tulip/GlGraphRenderer.h>

#include <map>

namespace tlp {

class Graph;
class GlScene;
struct OcclusionTest;

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

  virtual void selectEntities(Camera *camera,RenderingEntitiesFlag type, int x, int y, int w, int h, std::vector<SelectedEntity>& selectedEntities);

protected:

  void initSelectionRendering(RenderingEntitiesFlag type, int x, int y, int w, int h, std::map<unsigned int, SelectedEntity> &idMap,unsigned int &currentId);

  void buildSortedList();

  void drawLabelsForComplexEntities(bool drawSelected,OcclusionTest *occlusionTest,LayerLODUnit &layerLODUnit);

  GlLODCalculator *lodCalculator;


  GlScene *baseScene;
  GlScene *fakeScene;
  Vec4i selectionViewport;
};
}

#endif
///@endcond
