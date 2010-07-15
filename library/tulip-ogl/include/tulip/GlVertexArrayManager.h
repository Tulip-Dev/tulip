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
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>
#include <tulip/tuliphash.h>
#include <tulip/ObservableProperty.h>

namespace tlp {

  class GlEdge;
  class GlNode;
  class GlGraphInputData;

  /** \brief Class used to render edges/nodes with vertex array
   *
   * Class used to render edges/nodes with vertex array
   */
  class TLP_GL_SCOPE GlVertexArrayManager : public PropertyObserver {

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

    void addEdge(GlEdge *edge);

    void activateLineEdgeDisplay(GlEdge *edge, bool selected);

    void beforeSetNodeValue(PropertyInterface*, const node);
    void destroy(PropertyInterface*);

  protected:

    void clearData();
    void initObservers();
    void clearObservers();

    GlGraphInputData *inputData;
    Graph *graph;

    bool isBegin;
    bool toCompute;

    bool colorInterpolate;

    std::vector<Coord> linesCoordsArray;
    std::vector<Color> linesColorsArray;
    std::vector<GLint> linesIndexArray;
    std::vector<GLsizei> linesIndexCountArray;
    std::vector<GLint> linesRenderingIndexArray;
    std::vector<GLint> linesSelectedRenderingIndexArray;

    unsigned int currentIndex;

    TLP_HASH_MAP<unsigned int,std::pair<unsigned int,unsigned int> > edgeToLineIndexHashMap;
  };

}

#endif
