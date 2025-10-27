/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/OpenGlIncludes.h>

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Observable.h>
#include <tulip/GlSceneVisitor.h>

#include <map>
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
class TLP_GL_SCOPE GlVertexArrayManager : public GlSceneVisitor, private Observable {

public:
  /**
   * Constructor
   * \param inputData : input data to use for this GlVertexArrayManager
   */
  GlVertexArrayManager(GlGraphInputData *inputData);

  ~GlVertexArrayManager() override;

  /**
   * Method used for GlSimpleEntity
   */
  void visit(GlSimpleEntity *) override {}

  /**
   * Method used for GlNodes (and GlMetaNodes)
   */
  void visit(GlNode *glNode) override;

  /**
   * Method used for GlEdges
   */
  void visit(GlEdge *glEdge) override;

  void endOfVisit() override;

  void reserveMemoryForGraphElts(unsigned int nbNodes, unsigned int nbEdges) override;

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
   * Call this function if this GlVertexArrayManager have to compute layout property
   */
  void setHaveToComputeLayout(bool compute);
  /**
   * Call this function if this GlVertexArrayManager have to compute colors property
   */
  void setHaveToComputeColor(bool compute);

  /**
   * Call this function at the beginning of the rendering
   * This function clear entities to render
   */
  void beginRendering();
  /**
   * Call this function at the end of rendering
   * This function draws needed entities
   */
  void endRendering();

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
  void treatEvent(const Event &) override;

  void clearLayoutData();
  void clearColorData();
  void clearData();
  void initObservers();
  void clearObservers(PropertyInterface *deletedProperty = nullptr);

  GlGraphInputData *inputData;
  Graph *graph;
  // Store properties used to compute the arrays
  LayoutProperty *layoutProperty;
  SizeProperty *sizeProperty;
  IntegerProperty *shapeProperty;
  DoubleProperty *rotationProperty, *lengthRatioProperty;
  ColorProperty *colorProperty;
  ColorProperty *borderColorProperty;
  DoubleProperty *borderWidthProperty;
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

  bool edgesModified;
  bool colorInterpolate;
  bool sizeInterpolate;
  bool viewArrow;

  std::vector<Coord> linesCoordsArray;
  std::vector<Color> linesColorsArray;

  std::vector<GLuint> linesRenderingIndicesArray;
  std::vector<GLuint> linesSelectedRenderingIndicesArray;

  std::vector<Coord> quadsCoordsArray;
  std::vector<Color> quadsColorsArray;
  std::vector<Color> quadsOutlineColorsArray;

  std::vector<GLuint> quadsRenderingIndicesArray;
  std::vector<GLuint> quadsSelectedRenderingIndicesArray;

  std::map<float, std::vector<GLuint>> quadsOutlineRenderingIndicesArray;
  std::map<float, std::vector<GLuint>> quadsSelectedOutlineRenderingIndicesArray;

  std::vector<Coord> pointsCoordsArray;
  std::vector<Color> pointsColorsArray;

  std::vector<GLuint> pointsNodesRenderingIndexArray;
  std::vector<GLuint> pointsNodesSelectedRenderingIndexArray;
  std::vector<GLuint> pointsEdgesRenderingIndexArray;
  std::vector<GLuint> pointsEdgesSelectedRenderingIndexArray;

  struct edgeInfos {
    unsigned int linesIndex;
    std::vector<Coord> lineVertices;
    unsigned int quadsIndex;
    std::vector<Coord> quadVertices;
    std::vector<Color> lineColors;
    std::vector<Color> quadColors;
    Color edgeColor, borderColor;
  };

  std::vector<edgeInfos> edgeInfosVector;

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
} // namespace tlp

#endif
///@endcond
