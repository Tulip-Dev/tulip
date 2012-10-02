/*
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

#ifndef Tulip_GLVERTEXARRAYMANAGER_H
#define Tulip_GLVERTEXARRAYMANAGER_H

#include <cassert>
#include <iostream>
#include <tulip/OpenGlConfigManager.h>

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/PropertyInterface.h>

#include <vector>

namespace tlp {

class Graph;
class GlEdge;
class GlNode;
class GlGraphInputData;

/** \brief Class used to render edges/nodes with vertex array
 *
 * Class used to render edges/nodes with vertex array
 */
class TLP_GL_SCOPE GlVertexArrayManager : private Observable {

public:

  /**
   * Constructor
   * \param inputData : input data to use for this GlVertexArrayManager
   */
  GlVertexArrayManager(GlGraphInputData *inputData);

  ~GlVertexArrayManager();

  /**
   * Call this function when you want to change input data used by this GlVertexArrayManager
   */
  void setInputData(GlGraphInputData *inputData);

  /**
   * Return if this GlVertexArrayManager is used to render entities of the scene
   */
  inline bool renderingIsBegin() {
    return isBegin;
  }

  /**
   * Return if this GlVertexArrayManager have to compute its data
   */
  bool haveToCompute();

  /**
   * Call this function if this GlVertexArrayManager have to compute colors and layout properties
   */
  void setHaveToComputeAll(bool compute);
  /**
   * Call this function if this GlVertexArrayManager have to compute layout propertie
   */
  void setHaveToComputeLayout(bool compute);
  /**
   * Call this function if this GlVertexArrayManager have to compute colors propertie
   */
  void setHaveToComputeColor(bool compute);

  /**
   * Call this function at the begining of the rendering
   * This function clear entities to render
   */
  void beginRendering();
  /**
   * Call this funtion at the end of rendering
   * This function draw needed entities
   */
  void endRendering();

  /**
   * This function is call by GlVertexArraySceneVisitor to inform GlVertexArrayManager that we need to render an edge
   */
  void addEdge(GlEdge *edge);
  /**
   * This function is call by GlVertexArraySceneVisitor to inform GlVertexArrayManager that we need to render a node
   */
  void addNode(GlNode *node);

  /**
   * You can call this function to pause rendering
   * For example this function is call in GlMetaNodeTrueRenderer to don't use GlVertexArrayManager
   */
  void pauseRendering(bool pause);

  /**
   * You can call this function to deactivate/activate GlVertexArrayManager
   */
  void activate(bool act);

  bool isActivated() {
    return activated;
  }

  /**
   * This function is call when you want to activate line rendering of a specific edge
   */
  void activateLineEdgeDisplay(GlEdge *edge, bool selected);
  /**
   * This function is call when you want to activate quad rendering of a specific edge
   */
  void activateQuadEdgeDisplay(GlEdge *edge, bool selected);
  /**
   * This function is call when you want to activate point rendering of a specific edge
   */
  void activatePointEdgeDisplay(GlEdge *edge, bool selected);
  /**
   * This function is call when you want to activate point rendering of a specific node
   */
  void activatePointNodeDisplay(GlNode *node, bool onePixel, bool selected);

protected:

  void propertyValueChanged(tlp::PropertyInterface *property);
  void treatEvent(const Event&);

  void clearLayoutData();
  void clearColorData();
  void clearData();
  void initObservers();
  void clearObservers(PropertyInterface *deletedProperty=NULL);

  GlGraphInputData *inputData;
  Graph *graph;
  bool graphObserverActivated;
  bool layoutObserverActivated;
  bool colorObserverActivated;

  bool activated;
  bool isBegin;
  bool toComputeAll;
  bool toComputeLayout;
  bool toComputeColor;

  bool vectorLayoutSizeInit;
  bool vectorColorSizeInit;
  bool vectorIndexSizeInit;

  bool colorInterpolate;
  bool sizeInterpolate;
  bool edgesModified;

  std::vector<Coord> linesCoordsArray;
  std::vector<Color> linesColorsArray;
  std::vector<GLint> linesIndexArray;
  std::vector<GLsizei> linesIndexCountArray;

  std::vector<GLint> linesRenderingStartIndexArray;
  std::vector<GLsizei> linesRenderingCountArray;
  std::vector<GLint> linesSelectedRenderingStartIndexArray;
  std::vector<GLsizei> linesSelectedRenderingCountArray;

  std::vector<Coord> quadsCoordsArray;
  std::vector<Color> quadsColorsArray;
  std::vector<Color> quadsOutlineColorsArray;
  std::vector<GLint> quadsIndexArray;
  std::vector<GLsizei> quadsIndexCountArray;
  std::vector<std::vector<GLuint> > quadsBottomOutlineIndexArray;
  std::vector<std::vector<GLuint> > quadsTopOutlineIndexArray;

  std::vector<GLint> quadsRenderingStartIndexArray;
  std::vector<GLsizei> quadsRenderingCountArray;
  std::vector<GLint> quadsSelectedRenderingStartIndexArray;
  std::vector<GLsizei> quadsSelectedRenderingCountArray;
  std::map<float, std::vector<const GLuint* > > quadsOutlineRenderingIndexArray;
  std::map<float, std::vector<GLsizei> > quadsOutlineRenderingCountArray;
  std::map<float, std::vector<const GLuint* > > quadsOutlineSelectedRenderingIndexArray;
  std::map<float, std::vector<GLsizei> > quadsOutlineSelectedRenderingCountArray;

  std::vector<Coord> pointsCoordsArray;
  std::vector<Color> pointsColorsArray;

  std::vector<GLuint> points1PNodesRenderingIndexArray;
  std::vector<GLuint> points1PNodesSelectedRenderingIndexArray;
  std::vector<GLuint> points2PNodesRenderingIndexArray;
  std::vector<GLuint> points2PNodesSelectedRenderingIndexArray;
  std::vector<GLuint> points1PEdgesRenderingIndexArray;
  std::vector<GLuint> points1PEdgesSelectedRenderingIndexArray;

  std::vector<std::pair<unsigned int,unsigned int> > edgeToLineIndexVector;
  std::vector<std::pair<unsigned int,unsigned int> > edgeToQuadIndexVector;
  std::vector<unsigned int> edgeToBottomOulineIndexVector;
  std::vector<unsigned int> edgeToTopOutlineIndexVector;
  std::vector<unsigned int> edgeToPointIndexVector;
  std::vector<unsigned int> nodeToPointIndexVector;
};

}

#endif
///@endcond