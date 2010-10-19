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
#ifndef Tulip_GLVERTEXARRAYMANAGER_H
#define Tulip_GLVERTEXARRAYMANAGER_H

#include <cassert>
#include <iostream>
#include "tulip/OpenGlConfigManager.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>
#include <tulip/tuliphash.h>
#include <tulip/ObservableGraph.h>
#include <tulip/ObservableProperty.h>

namespace tlp {

  class GlEdge;
  class GlNode;
  class GlGraphInputData;

  /** \brief Class used to render edges/nodes with vertex array
   *
   * Class used to render edges/nodes with vertex array
   */
  class TLP_GL_SCOPE GlVertexArrayManager : public GraphObserver, public PropertyObserver {

  public:

    GlVertexArrayManager(GlGraphInputData *inputData);

    ~GlVertexArrayManager();

    inline bool renderingIsBegin(){
      return isBegin;
    }

    bool haveToCompute();

    inline void setHaveToCompute(bool compute){
      if(compute)
        clearObservers();
      if(!compute)
        initObservers();

      toCompute=compute;
    }

    void beginRendering();
    void endRendering();

    void pauseRendering(bool pause);
    void activate(bool act);

    void addEdge(GlEdge *edge);
    void addNode(GlNode *node);

    void activateLineEdgeDisplay(GlEdge *edge, bool selected);
    void activatePointEdgeDisplay(GlEdge *edge, bool selected);
    void activatePointNodeDisplay(GlNode *node, bool onePixel, bool selected);

    void addEdge(Graph *,const edge);
    void beforeSetNodeValue(PropertyInterface*, const node);
    void beforeSetEdgeValue(PropertyInterface*, const edge);

    void destroy(Graph *);
    void destroy(PropertyInterface*);

  protected:

    void clearData();
    void initObservers();
    void clearObservers();

    GlGraphInputData *inputData;
    Graph *graph;
    bool observersActivated;

    bool activated;
    bool isBegin;
    bool toCompute;
    bool vectorSizeInit;

    bool colorInterpolate;

    std::vector<Coord> linesCoordsArray;
    std::vector<Color> linesColorsArray;
    std::vector<GLint> linesIndexArray;
    std::vector<GLsizei> linesIndexCountArray;
    std::vector<GLint> linesRenderingIndexArray;
    std::vector<GLint> linesSelectedRenderingIndexArray;

    std::vector<Coord> pointsCoordsArray;
    std::vector<Color> pointsColorsArray;
    std::vector<GLint> points1PRenderingIndexArray;
    std::vector<GLint> points1PSelectedRenderingIndexArray;
    std::vector<GLint> points2PRenderingIndexArray;
    std::vector<GLint> points2PSelectedRenderingIndexArray;


    std::vector<std::pair<unsigned int,unsigned int> > edgeToLineIndexVector;
    std::vector<unsigned int> edgeToPointIndexVector;
    std::vector<unsigned int> nodeToPointIndexVector;
  };

}

#endif
