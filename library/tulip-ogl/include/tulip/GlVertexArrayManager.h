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

#ifndef Tulip_GLVERTEXARRAYMANAGER_H
#define Tulip_GLVERTEXARRAYMANAGER_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Observable.h>
#include <tulip/tuliphash.h>

#include <vector>

namespace tlp {

class Graph;
class GlEdge;
class GlNode;
class GlGraphInputData;
class PropertyInterface;
class ColorProperty;
class LayoutProperty;
class SizeProperty;
class IntegerProperty;
class DoubleProperty;

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
  void activatePointNodeDisplay(GlNode *node, bool selected);

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
  //Store properties used to compute the arrays
  LayoutProperty* layoutProperty;
  SizeProperty* sizeProperty;
  IntegerProperty* shapeProperty;
  DoubleProperty* rotationProperty;
  ColorProperty* colorProperty;
  ColorProperty* borderColorProperty;
  DoubleProperty* borderWidthProperty;
  IntegerProperty *srcAnchorShapeProperty;
  IntegerProperty *tgtAnchorShapeProperty;
  SizeProperty *srcAnchorSizeProperty;
  SizeProperty *tgtAnchorSizeProperty;
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

  bool edgesModified;
  bool colorInterpolate;
  bool sizeInterpolate;
  bool viewArrow;


  std::vector<Coord> linesCoordsArray;
  std::vector<Color> linesColorsArray;
  std::vector<GLint> linesIndexArray;
  std::vector<GLsizei> linesIndexCountArray;

  std::vector<GLuint> linesRenderingIndicesArray;
  std::vector<GLuint> linesSelectedRenderingIndicesArray;

  std::vector<Coord> quadsCoordsArray;
  std::vector<Color> quadsColorsArray;
  std::vector<Color> quadsOutlineColorsArray;
  std::vector<GLuint> quadsIndexArray;
  std::vector<GLsizei> quadsIndexCountArray;
  std::vector<std::vector<GLuint> > quadsBottomOutlineIndexArray;
  std::vector<std::vector<GLuint> > quadsTopOutlineIndexArray;

  std::vector<GLuint> quadsRenderingIndicesArray;
  std::vector<GLuint> quadsSelectedRenderingIndicesArray;

  std::map<float, std::vector<GLuint> > quadsOutlineRenderingIndicesArray;
  std::map<float, std::vector<GLuint> > quadsSelectedOutlineRenderingIndicesArray;

  std::vector<Coord> pointsCoordsArray;
  std::vector<Color> pointsColorsArray;

  std::vector<GLuint> pointsNodesRenderingIndexArray;
  std::vector<GLuint> pointsNodesSelectedRenderingIndexArray;
  std::vector<GLuint> pointsEdgesRenderingIndexArray;
  std::vector<GLuint> pointsEdgesSelectedRenderingIndexArray;

  TLP_HASH_MAP<unsigned int, std::pair<unsigned int,unsigned int> > edgeToLineIndexVector;
  TLP_HASH_MAP<unsigned int, std::pair<unsigned int,unsigned int> > edgeToQuadIndexVector;
  TLP_HASH_MAP<unsigned int, unsigned int> edgeToBottomOulineIndexVector;
  TLP_HASH_MAP<unsigned int, unsigned int> edgeToTopOutlineIndexVector;
  TLP_HASH_MAP<unsigned int, unsigned int> edgeToPointIndexVector;
  TLP_HASH_MAP<unsigned int, unsigned int> nodeToPointIndexVector;

  GLuint pointsVerticesVBO;
  GLuint pointsColorsVBO;
  GLuint linesVerticesVBO;
  GLuint linesColorsVBO;
  GLuint quadsVerticesVBO;
  GLuint quadsColorsVBO;
  GLuint quadsOutlineColorsVBO;

  bool pointsVerticesUploaded;
  bool pointsColorsUploaded;
  bool linesVerticesUploaded;
  bool linesColorsUploaded;
  bool quadsVerticesUploaded;
  bool quadsColorsUploaded;
  bool quadsOutlineColorsUploaded;

  bool verticesUploadNeeded;
  bool colorsUploadNeeded;

};

}

#endif
///@endcond
