/**
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

#include <GL/glew.h>

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlEdge.h>
#include <tulip/GlNode.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Graph.h>
#include <tulip/Curves.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlGraphRenderingParameters.h>

using namespace std;

// macro to get access to the underlying memory array of a vector
#if (__GNUC__ < 4 || __GNUC_MINOR__ < 2)
#define VECTOR_DATA(vec) &vec[0]
#else
#define VECTOR_DATA(vec) vec.data()
#endif

static bool isOpenGlOutOfMemory() {
  return glGetError() == GL_OUT_OF_MEMORY;
}

namespace tlp {
GlVertexArrayManager::GlVertexArrayManager(GlGraphInputData *i):inputData(i),graph(inputData->getGraph()),
  layoutProperty(inputData->getElementLayout()),
  sizeProperty(inputData->getElementSize()),
  shapeProperty(inputData->getElementShape()),
  rotationProperty(inputData->getElementRotation()),
  colorProperty(inputData->getElementColor()),
  borderColorProperty(inputData->getElementBorderColor()),
  borderWidthProperty(inputData->getElementBorderWidth()),
  srcAnchorShapeProperty(inputData->getElementSrcAnchorShape()),
  tgtAnchorShapeProperty(inputData->getElementTgtAnchorShape()),
  srcAnchorSizeProperty(inputData->getElementSrcAnchorSize()),
  tgtAnchorSizeProperty(inputData->getElementTgtAnchorSize()),
  graphObserverActivated(false),
  layoutObserverActivated(false),
  colorObserverActivated(false),
  activated(true),
  isBegin(false),
  toComputeAll(true),
  toComputeLayout(true),
  toComputeColor(true),
  vectorLayoutSizeInit(false),
  vectorColorSizeInit(false),
  vectorIndexSizeInit(false),
  edgesModified(false),
  colorInterpolate(inputData->parameters->isEdgeColorInterpolate()),
  sizeInterpolate(inputData->parameters->isEdgeSizeInterpolate()),
  viewArrow(inputData->parameters->isViewArrow()),
  pointsVerticesVBO(0), pointsColorsVBO(0), linesVerticesVBO(0), linesColorsVBO(0),
  quadsVerticesVBO(0), quadsColorsVBO(0), quadsOutlineColorsVBO(0),
  pointsVerticesUploaded(false), pointsColorsUploaded(false),
  linesVerticesUploaded(false), linesColorsUploaded(false),
  quadsVerticesUploaded(false), quadsColorsUploaded(false),
  quadsOutlineColorsUploaded(false),
  verticesUploadNeeded(true),
  colorsUploadNeeded(true) {
}

GlVertexArrayManager::~GlVertexArrayManager() {
  clearObservers();
  clearData();
  bool canUseVBO = OpenGlConfigManager::getInst().hasVertexBufferObject();

  if (canUseVBO && pointsVerticesVBO != 0) {
    glDeleteBuffers(1, &pointsVerticesVBO);
    glDeleteBuffers(1, &pointsColorsVBO);
    glDeleteBuffers(1, &linesVerticesVBO);
    glDeleteBuffers(1, &linesColorsVBO);
    glDeleteBuffers(1, &quadsVerticesVBO);
    glDeleteBuffers(1, &quadsColorsVBO);
    glDeleteBuffers(1, &quadsOutlineColorsVBO);
  }
}

void GlVertexArrayManager::setInputData(GlGraphInputData *inputData) {
  clearObservers();
  this->inputData=inputData;
  //Update properties with new ones
  layoutProperty = inputData->getElementLayout();
  sizeProperty = inputData->getElementSize();
  shapeProperty = inputData->getElementShape();
  rotationProperty = inputData->getElementRotation();
  colorProperty = inputData->getElementColor();
  borderColorProperty = inputData->getElementBorderColor();
  borderWidthProperty = inputData->getElementBorderWidth();
  srcAnchorShapeProperty = inputData->getElementSrcAnchorShape();
  tgtAnchorShapeProperty = inputData->getElementTgtAnchorShape();
  srcAnchorSizeProperty = inputData->getElementSrcAnchorSize();
  tgtAnchorSizeProperty = inputData->getElementTgtAnchorSize();
  colorInterpolate = inputData->parameters->isEdgeColorInterpolate();
  sizeInterpolate = inputData->parameters->isEdgeSizeInterpolate();
  viewArrow = inputData->parameters->isViewArrow();
  graph=inputData->getGraph();
  initObservers();
}

bool GlVertexArrayManager::haveToCompute() {
  bool recompute = false;

  if(toComputeAll || toComputeLayout || toComputeColor) {
    recompute = true;
  }

  if(inputData->parameters->isEdgeColorInterpolate()!=colorInterpolate) {
    colorInterpolate=inputData->parameters->isEdgeColorInterpolate();
    clearColorData();
    recompute = true;
  }

  if(inputData->parameters->isEdgeSizeInterpolate()!=sizeInterpolate) {
    sizeInterpolate=inputData->parameters->isEdgeSizeInterpolate();
    clearLayoutData();
    recompute = true;
  }

  if (inputData->parameters->isViewArrow() != viewArrow) {
    viewArrow = inputData->parameters->isViewArrow();
    clearLayoutData();
    recompute = true;
  }

  //The layout property in the input data has changed => need to recompute layout.
  if(layoutProperty != inputData->getElementLayout()) {
    if(layoutProperty && layoutObserverActivated)
      layoutProperty->removeListener(this);

    layoutProperty = inputData->getElementLayout();
    layoutProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  //Size property changed
  if(sizeProperty != inputData->getElementSize()) {
    if(sizeProperty && layoutObserverActivated)
      sizeProperty->removeListener(this);

    sizeProperty = inputData->getElementSize();
    sizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  //Shape property changed
  if(shapeProperty != inputData->getElementShape()) {
    if(shapeProperty && layoutObserverActivated)
      shapeProperty->removeListener(this);

    shapeProperty = inputData->getElementShape();
    shapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  //Rotation property changed
  if(rotationProperty != inputData->getElementRotation()) {
    if(rotationProperty && layoutObserverActivated)
      rotationProperty->removeListener(this);

    rotationProperty = inputData->getElementRotation();
    rotationProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  //Color property changed
  if(colorProperty != inputData->getElementColor()) {
    if(colorProperty && colorObserverActivated)
      colorProperty->removeListener(this);

    colorProperty = inputData->getElementColor();
    colorProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  //Color property changed
  if(borderColorProperty != inputData->getElementBorderColor()) {
    if(borderColorProperty && colorObserverActivated)
      borderColorProperty->removeListener(this);

    borderColorProperty = inputData->getElementBorderColor();
    borderColorProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  //Border width property changed
  if(borderWidthProperty != inputData->getElementBorderWidth()) {
    if(borderWidthProperty && colorObserverActivated)
      borderWidthProperty->removeListener(this);

    borderWidthProperty = inputData->getElementBorderWidth();
    borderWidthProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  if(srcAnchorShapeProperty != inputData->getElementSrcAnchorShape()) {
    if(srcAnchorShapeProperty && layoutObserverActivated)
      srcAnchorShapeProperty->removeListener(this);

    srcAnchorShapeProperty = inputData->getElementSrcAnchorShape();
    srcAnchorShapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if(tgtAnchorShapeProperty != inputData->getElementTgtAnchorShape()) {
    if(tgtAnchorShapeProperty && layoutObserverActivated)
      tgtAnchorShapeProperty->removeListener(this);

    tgtAnchorShapeProperty = inputData->getElementTgtAnchorShape();
    tgtAnchorShapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if(srcAnchorSizeProperty != inputData->getElementSrcAnchorSize()) {
    if(srcAnchorSizeProperty && layoutObserverActivated)
      srcAnchorSizeProperty->removeListener(this);

    srcAnchorSizeProperty = inputData->getElementSrcAnchorSize();
    srcAnchorSizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if(tgtAnchorSizeProperty != inputData->getElementTgtAnchorSize()) {
    if(tgtAnchorSizeProperty && layoutObserverActivated)
      tgtAnchorSizeProperty->removeListener(this);

    tgtAnchorSizeProperty = inputData->getElementTgtAnchorSize();
    tgtAnchorSizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  return recompute;
}

void GlVertexArrayManager::setHaveToComputeAll(bool compute) {
  if(compute) {
    clearObservers();
    clearData();
  }

  if(!compute)
    initObservers();

  toComputeAll=compute;
  toComputeLayout=compute;
  toComputeColor=compute;
}

void GlVertexArrayManager::setHaveToComputeLayout(bool compute) {
  toComputeLayout=compute;
}

void GlVertexArrayManager::setHaveToComputeColor(bool compute) {
  toComputeColor=compute;
}

void GlVertexArrayManager::beginRendering() {
  if(!activated)
    return;

  isBegin=true;

  linesSelectedRenderingIndicesArray.clear();
  linesRenderingIndicesArray.clear();

  quadsRenderingIndicesArray.clear();
  quadsSelectedRenderingIndicesArray.clear();

  quadsOutlineRenderingIndicesArray.clear();
  quadsSelectedOutlineRenderingIndicesArray.clear();

  pointsNodesRenderingIndexArray.clear();
  pointsNodesSelectedRenderingIndexArray.clear();
  pointsEdgesRenderingIndexArray.clear();
  pointsEdgesSelectedRenderingIndexArray.clear();

  pointsNodesRenderingIndexArray.reserve(graph->numberOfNodes());
  pointsNodesSelectedRenderingIndexArray.reserve(graph->numberOfNodes());

  pointsEdgesRenderingIndexArray.reserve(graph->numberOfEdges());
  pointsEdgesSelectedRenderingIndexArray.reserve(graph->numberOfEdges());

  if(!vectorLayoutSizeInit) {
    linesCoordsArray.reserve(graph->numberOfEdges()*2);
    quadsCoordsArray.reserve(graph->numberOfEdges()*4);
    pointsCoordsArray.reserve(graph->numberOfEdges()+graph->numberOfNodes());

    vectorLayoutSizeInit=true;
  }

  if(!vectorColorSizeInit) {
    linesColorsArray.reserve(graph->numberOfEdges()*2);
    quadsColorsArray.reserve(graph->numberOfEdges()*4);
    pointsColorsArray.reserve(graph->numberOfEdges()+graph->numberOfNodes());

    vectorColorSizeInit=true;
  }

  if(!vectorIndexSizeInit) {
    linesIndexArray.reserve(graph->numberOfEdges()*2);
    linesIndexCountArray.reserve(graph->numberOfEdges());

    vectorIndexSizeInit=true;
  }
}

void GlVertexArrayManager::endRendering() {
  if(!isBegin)
    return;

  isBegin=false;

  static bool canUseVBO = OpenGlConfigManager::getInst().hasVertexBufferObject();

  if (canUseVBO && quadsVerticesVBO == 0) {
    glGenBuffers(1, &pointsVerticesVBO);
    glGenBuffers(1, &pointsColorsVBO);
    glGenBuffers(1, &linesVerticesVBO);
    glGenBuffers(1, &linesColorsVBO);
    glGenBuffers(1, &quadsVerticesVBO);
    glGenBuffers(1, &quadsColorsVBO);
    glGenBuffers(1, &quadsOutlineColorsVBO);
  }

  if (canUseVBO && verticesUploadNeeded) {
    glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, pointsCoordsArray.size() * 3 * sizeof(float), VECTOR_DATA(pointsCoordsArray), GL_STATIC_DRAW);
    pointsVerticesUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, linesVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, linesCoordsArray.size() * 3 * sizeof(float), VECTOR_DATA(linesCoordsArray), GL_STATIC_DRAW);
    linesVerticesUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
    glBufferData(GL_ARRAY_BUFFER, quadsCoordsArray.size() * 3 * sizeof(float), VECTOR_DATA(quadsCoordsArray), GL_STATIC_DRAW);
    quadsVerticesUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    verticesUploadNeeded = false;
  }

  if (canUseVBO && colorsUploadNeeded) {
    glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, pointsColorsArray.size() * 4 * sizeof(unsigned char), VECTOR_DATA(pointsColorsArray), GL_STATIC_DRAW);
    pointsColorsUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, linesColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, linesColorsArray.size() * 4 * sizeof(unsigned char), VECTOR_DATA(linesColorsArray), GL_STATIC_DRAW);
    linesColorsUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, quadsColorsArray.size() * 4 * sizeof(unsigned char), VECTOR_DATA(quadsColorsArray), GL_STATIC_DRAW);
    quadsColorsUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, quadsOutlineColorsVBO);
    glBufferData(GL_ARRAY_BUFFER, quadsOutlineColorsArray.size() * 4 * sizeof(unsigned char), VECTOR_DATA(quadsOutlineColorsArray), GL_STATIC_DRAW);
    quadsOutlineColorsUploaded = !isOpenGlOutOfMemory();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    colorsUploadNeeded = false;
  }

  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_COLOR_MATERIAL);

  glEnableClientState(GL_VERTEX_ARRAY);

  //============ Graph elements rendering ============================

  glEnableClientState(GL_COLOR_ARRAY);

  // Edges point rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
  glPointSize(2);

  if (!pointsEdgesRenderingIndexArray.empty()) {
    if (canUseVBO && pointsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    if (canUseVBO && pointsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    }
    else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
    }

    glDrawElements(GL_POINTS, pointsEdgesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(pointsEdgesRenderingIndexArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Nodes point rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getNodesStencil(), 0xFFFF);
  glPointSize(4);

  if (!pointsNodesRenderingIndexArray.empty()) {
    if (canUseVBO && pointsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    if (canUseVBO && pointsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    }
    else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
    }

    glDrawElements(GL_POINTS, pointsNodesRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(pointsNodesRenderingIndexArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Edges polyline rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getEdgesStencil(), 0xFFFF);
  glLineWidth(1.4f);

  if (!linesRenderingIndicesArray.empty()) {
    if (canUseVBO && linesVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, linesVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
    }

    if (canUseVBO && linesColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, linesColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    }
    else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(linesColorsArray));
    }

    glDrawElements(GL_LINES, linesRenderingIndicesArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Edges quad rendering
  if (!quadsRenderingIndicesArray.empty()) {
    if (canUseVBO && quadsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
    }

    if (!inputData->parameters->isEdgeColorInterpolate()) {
      if (canUseVBO && quadsOutlineColorsUploaded) {
        glBindBuffer(GL_ARRAY_BUFFER, quadsOutlineColorsVBO);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
      }
      else {
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsOutlineColorsArray));
      }
    }
    else {
      if (canUseVBO && quadsColorsUploaded) {
        glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
      }
      else {
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
      }
    }

    for (map<float, vector<GLuint> >::const_iterator it = quadsOutlineRenderingIndicesArray.begin(); it != quadsOutlineRenderingIndicesArray.end() ; ++it) {
      glLineWidth(it->first);
      glDrawElements(GL_LINES, it->second.size(), GL_UNSIGNED_INT, VECTOR_DATA(it->second));
    }

    if (canUseVBO && quadsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    }
    else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
    }

    glDrawElements(GL_TRIANGLES, quadsRenderingIndicesArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(quadsRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  glDisableClientState(GL_COLOR_ARRAY);

  //============ Selected graph elements rendering ============================

  Color selectionColor=inputData->parameters->getSelectionColor();
  glColor4ubv(reinterpret_cast<const GLubyte *>(&selectionColor));

  // Selected edges point rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
  glPointSize(2);

  if (!pointsEdgesSelectedRenderingIndexArray.empty()) {
    if (canUseVBO && pointsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    glDrawElements(GL_POINTS, pointsEdgesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(pointsEdgesSelectedRenderingIndexArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Selected nodes point rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedNodesStencil(), 0xFFFF);
  glPointSize(4);

  if (!pointsNodesSelectedRenderingIndexArray.empty()) {
    if (canUseVBO && pointsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    glDrawElements(GL_POINTS, pointsNodesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(pointsNodesSelectedRenderingIndexArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Selected edges polyline rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
  glLineWidth(4);

  if (!linesSelectedRenderingIndicesArray.empty()) {
    if (canUseVBO && linesVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, linesVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
    }

    glDrawElements(GL_LINES, linesSelectedRenderingIndicesArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(linesSelectedRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Selected edges quad rendering
  if (!quadsSelectedRenderingIndicesArray.empty()) {
    if (canUseVBO && quadsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    }
    else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
    }

    glDrawElements(GL_TRIANGLES, quadsSelectedRenderingIndicesArray.size(), GL_UNSIGNED_INT, VECTOR_DATA(quadsSelectedRenderingIndicesArray));

    for (map<float, vector<GLuint> >::iterator it = quadsSelectedOutlineRenderingIndicesArray.begin(); it != quadsSelectedOutlineRenderingIndicesArray.end() ; ++it) {
      glLineWidth(it->first);
      glDrawElements(GL_LINES, it->second.size(), GL_UNSIGNED_INT, VECTOR_DATA(it->second));
    }

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glPointSize(1);
  glLineWidth(1);
}

void GlVertexArrayManager::pauseRendering(bool pause) {
  isBegin=!pause;
}

void GlVertexArrayManager::activate(bool act) {
  activated=act;
}

void GlVertexArrayManager::addEdge(GlEdge *gledge) {

  edge e(gledge->id);
  const pair<node, node> &ends=graph->ends(e);
  node src = ends.first;
  node tgt = ends.second;

  const Coord &srcCoord = layoutProperty->getNodeValue(src);
  const Coord &tgtCoord = layoutProperty->getNodeValue(tgt);

  const Size &srcSize = sizeProperty->getNodeValue(src);
  const Size &tgtSize = sizeProperty->getNodeValue(tgt);

  const Color &edgeColor = colorProperty->getEdgeValue(e);
  const Color &srcColor = colorProperty->getNodeValue(src);
  const Color &tgtColor = colorProperty->getNodeValue(tgt);
  const Color &borderColor = borderColorProperty->getEdgeValue(e);

  if(toComputeLayout) {

    size_t lastIndex=linesCoordsArray.size();
    gledge->getVertices(inputData,linesCoordsArray);
    size_t numberOfVertices=linesCoordsArray.size()-lastIndex;

    vector<Coord> vertices;

    if(numberOfVertices!=0) {
      edgeToLineIndexVector[gledge->id]=pair<unsigned int,unsigned int>(linesIndexArray.size(),linesIndexCountArray.size());
      linesIndexArray.push_back(lastIndex);

      for(size_t i=0; i<numberOfVertices; ++i) {
        vertices.push_back(linesCoordsArray[lastIndex+i]);
      }

      linesIndexCountArray.push_back(numberOfVertices);

      pointsCoordsArray.push_back(linesCoordsArray[lastIndex]);

      edgeToPointIndexVector[gledge->id]=pointsCoordsArray.size()-1;

      Size edgeSize;
      float maxSrcSize, maxTgtSize;

      if(srcSize[0]>=srcSize[1])
        maxSrcSize=srcSize[0];
      else
        maxSrcSize=srcSize[1];

      if(tgtSize[0]>=tgtSize[1])
        maxTgtSize=tgtSize[0];
      else
        maxTgtSize=tgtSize[1];

      gledge->getEdgeSize(inputData, e,srcSize,tgtSize,maxSrcSize,maxTgtSize,edgeSize);

      vector<float> edgeSizes;
      getSizes(vertices, edgeSize[0]/2.0f, edgeSize[1]/2.0f, edgeSizes);

      edgeToQuadIndexVector[gledge->id]=pair<unsigned int,unsigned int>(quadsIndexArray.size(),quadsIndexCountArray.size());
      edgeToBottomOulineIndexVector[gledge->id]=quadsBottomOutlineIndexArray.size();
      edgeToTopOutlineIndexVector[gledge->id]=quadsTopOutlineIndexArray.size();

      size_t lastQuadIndex=quadsCoordsArray.size();
      quadsIndexArray.push_back(lastQuadIndex);

      vector<Coord> quadVertices;
      buildCurvePoints(vertices, edgeSizes, layoutProperty->getNodeValue(src), layoutProperty->getNodeValue(tgt), quadVertices);

      vector<GLuint> quadBottomOutlineIndices;
      vector<GLuint> quadTopOutlineIndices;

      for (size_t i = 0 ; i < quadVertices.size() / 2 ; ++i) {
        quadBottomOutlineIndices.push_back(lastQuadIndex+2*i);
        quadTopOutlineIndices.push_back(lastQuadIndex+2*i+1);
      }

      quadsBottomOutlineIndexArray.push_back(quadBottomOutlineIndices);
      quadsTopOutlineIndexArray.push_back(quadTopOutlineIndices);

      quadsCoordsArray.insert(quadsCoordsArray.end(), quadVertices.begin(), quadVertices.end());
      quadsIndexCountArray.push_back(quadVertices.size());

      const vector<Coord> &bends = layoutProperty->getEdgeValue(e);
      Coord srcAnchor, tgtAnchor;
      gledge->getEdgeAnchor(inputData,src,tgt,bends,srcCoord,tgtCoord,srcSize,tgtSize,srcAnchor,tgtAnchor);

      linesCoordsArray[lastIndex] = srcAnchor;
      linesCoordsArray[lastIndex+numberOfVertices-1] = tgtAnchor;
    }
    else {
      linesIndexCountArray.push_back(0);
      edgeToPointIndexVector[gledge->id]=-1;
      quadsIndexCountArray.push_back(0);
      edgeToLineIndexVector[gledge->id]=make_pair(-1,linesIndexCountArray.size()-1);
      edgeToQuadIndexVector[gledge->id] = make_pair(-1,quadsIndexCountArray.size()-1);
    }

  }

  if (toComputeColor) {
    size_t lastIndex=linesColorsArray.size();
    pair<unsigned int,unsigned int> index=edgeToLineIndexVector[gledge->id];
    pair<unsigned int,unsigned int> indexQuad=edgeToQuadIndexVector[gledge->id];
    GLsizei numberOfVertices=linesIndexCountArray[index.second];

    if(numberOfVertices!=0) {
      vector<Color> colors;
      gledge->getColors(inputData, &linesCoordsArray[linesIndexArray[index.first]],numberOfVertices,colors);
      linesColorsArray.insert(linesColorsArray.end(), colors.begin(), colors.end());

      GLsizei numberQuadVertices=quadsIndexCountArray[indexQuad.second];
      unsigned int baseQuadIndex=quadsIndexArray[indexQuad.first];

      vector<Coord> centerLine;
      centerLine.reserve(numberQuadVertices/2);

      for (int i = 0 ; i < numberQuadVertices / 2 ; ++i) {
        centerLine.push_back((quadsCoordsArray[baseQuadIndex+2*i]+quadsCoordsArray[2*i+1])/2.f);
      }

      colors.clear();
      getColors(&centerLine[0], centerLine.size(), srcColor, tgtColor,colors);

      for (size_t i = 0 ; i < colors.size() ; ++i) {
        if (inputData->parameters->isEdgeColorInterpolate()) {
          quadsColorsArray.push_back(colors[i]);
          quadsColorsArray.push_back(colors[i]);
        }
        else {
          quadsColorsArray.push_back(edgeColor);
          quadsColorsArray.push_back(edgeColor);
        }

        quadsOutlineColorsArray.push_back(borderColor);
        quadsOutlineColorsArray.push_back(borderColor);
      }

      pointsColorsArray.push_back(linesColorsArray[lastIndex]);
    }
  }
}

void GlVertexArrayManager::addNode(GlNode *node) {
  if(toComputeLayout) {
    vector<Color> tmp;
    node->getPointAndColor(inputData,pointsCoordsArray, tmp);
    nodeToPointIndexVector[node->id]=pointsCoordsArray.size()-1;
  }

  if (toComputeColor) {
    node->getColor(inputData,pointsColorsArray);
  }
}

void GlVertexArrayManager::activateLineEdgeDisplay(GlEdge *edge,bool selected) {
  pair<unsigned int,unsigned int> index=edgeToLineIndexVector[edge->id];
  unsigned int numberOfVertices=linesIndexCountArray[index.second];

  if(numberOfVertices==0)
    return;

  unsigned int baseIndex=linesIndexArray[index.first];

  if(!selected) {
    for (unsigned int i = baseIndex ; i < baseIndex + numberOfVertices - 1 ; ++i) {
      linesRenderingIndicesArray.push_back(i);
      linesRenderingIndicesArray.push_back(i+1);
    }
  }
  else {
    for (unsigned int i = baseIndex ; i < baseIndex + numberOfVertices - 1 ; ++i) {
      linesSelectedRenderingIndicesArray.push_back(i);
      linesSelectedRenderingIndicesArray.push_back(i+1);
    }
  }
}

void GlVertexArrayManager::activateQuadEdgeDisplay(GlEdge *edge, bool selected) {
  pair<unsigned int,unsigned int> index=edgeToQuadIndexVector[edge->id];
  unsigned int numberOfVertices=quadsIndexCountArray[index.second];
  unsigned int bottomOutlineIndicesIdx = edgeToBottomOulineIndexVector[edge->id];
  unsigned int topOutlineIndicesIdx = edgeToTopOutlineIndexVector[edge->id];

  float borderWidth = static_cast<float>(inputData->getElementBorderWidth()->getEdgeValue(tlp::edge(edge->id)));

  if(numberOfVertices==0)
    return;

  unsigned int baseIndex=quadsIndexArray[index.first];

  if (!selected) {

    for (unsigned int i = baseIndex ; i < baseIndex + numberOfVertices - 2 ; i+=2) {
      quadsRenderingIndicesArray.push_back(i);
      quadsRenderingIndicesArray.push_back(i+1);
      quadsRenderingIndicesArray.push_back(i+2);

      quadsRenderingIndicesArray.push_back(i+2);
      quadsRenderingIndicesArray.push_back(i+1);
      quadsRenderingIndicesArray.push_back(i+3);
    }

    if (borderWidth > 0) {

      if (quadsOutlineRenderingIndicesArray.find(borderWidth) == quadsOutlineRenderingIndicesArray.end()) {
        quadsOutlineRenderingIndicesArray[borderWidth] = vector<GLuint>();
      }

      for (unsigned int i = 0 ; i < quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx].size() - 1 ; ++i) {
        quadsOutlineRenderingIndicesArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][i]);
        quadsOutlineRenderingIndicesArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][i+1]);
      }

      for (unsigned int i = 0 ; i < quadsTopOutlineIndexArray[topOutlineIndicesIdx].size() - 1 ; ++i) {
        quadsOutlineRenderingIndicesArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx][i]);
        quadsOutlineRenderingIndicesArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx][i+1]);
      }

    }
  }
  else {
    for (unsigned int i = baseIndex ; i < baseIndex + numberOfVertices - 2 ; i+=2) {
      quadsSelectedRenderingIndicesArray.push_back(i);
      quadsSelectedRenderingIndicesArray.push_back(i+1);
      quadsSelectedRenderingIndicesArray.push_back(i+2);

      quadsSelectedRenderingIndicesArray.push_back(i+2);
      quadsSelectedRenderingIndicesArray.push_back(i+1);
      quadsSelectedRenderingIndicesArray.push_back(i+3);
    }

    if (borderWidth > 0) {

      if (quadsSelectedOutlineRenderingIndicesArray.find(borderWidth) == quadsSelectedOutlineRenderingIndicesArray.end()) {
        quadsSelectedOutlineRenderingIndicesArray[borderWidth] = vector<GLuint>();
      }

      for (unsigned int i = 0 ; i < quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx].size() - 1 ; ++i) {
        quadsSelectedOutlineRenderingIndicesArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][i]);
        quadsSelectedOutlineRenderingIndicesArray[borderWidth].push_back(quadsBottomOutlineIndexArray[bottomOutlineIndicesIdx][i+1]);
      }

      for (unsigned int i = 0 ; i < quadsTopOutlineIndexArray[topOutlineIndicesIdx].size() - 1 ; ++i) {
        quadsSelectedOutlineRenderingIndicesArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx][i]);
        quadsSelectedOutlineRenderingIndicesArray[borderWidth].push_back(quadsTopOutlineIndexArray[topOutlineIndicesIdx][i+1]);
      }

    }
  }
}

void GlVertexArrayManager::activatePointEdgeDisplay(GlEdge *edge,bool selected) {
  unsigned int index=edgeToPointIndexVector[edge->id];

  if(index==(unsigned int)(-1))
    return;

  if(!selected) {
    pointsEdgesRenderingIndexArray.push_back(index);
  }
  else {
    pointsEdgesSelectedRenderingIndexArray.push_back(index);
  }
}

void GlVertexArrayManager::activatePointNodeDisplay(GlNode *node, bool selected) {
  unsigned int index=nodeToPointIndexVector[node->id];

  if(index==(unsigned int)(-1))
    return;

  if(!selected) {
    pointsNodesRenderingIndexArray.push_back(index);
  }
  else {
    pointsNodesSelectedRenderingIndexArray.push_back(index);
  }
}

void GlVertexArrayManager::propertyValueChanged(PropertyInterface *property) {
  if(layoutProperty==property || sizeProperty==property || shapeProperty==property|| rotationProperty==property ||
      srcAnchorShapeProperty==property || tgtAnchorShapeProperty==property || srcAnchorSizeProperty==property || tgtAnchorSizeProperty==property) {
    setHaveToComputeLayout(true);
    clearLayoutData();
    layoutProperty->removeListener(this);
    sizeProperty->removeListener(this);
    shapeProperty->removeListener(this);
    rotationProperty->removeListener(this);
    layoutObserverActivated=false;
  }

  if(edgesModified || layoutProperty==property || colorProperty==property || borderColorProperty==property || borderWidthProperty==property) {
    setHaveToComputeColor(true);
    clearColorData();
    colorProperty->removeListener(this);
    borderColorProperty->removeListener(this);
    colorObserverActivated=false;
  }

  edgesModified = false;
}

void GlVertexArrayManager::clearLayoutData() {
  toComputeLayout=true;
  verticesUploadNeeded=true;

  linesCoordsArray.clear();
  pointsCoordsArray.clear();
  quadsCoordsArray.clear();

  linesIndexArray.clear();
  linesIndexCountArray.clear();

  if (!edgeToLineIndexVector.empty())
    edgeToLineIndexVector.clear();

  if (!edgeToQuadIndexVector.empty())
    edgeToQuadIndexVector.clear();

  if (!edgeToTopOutlineIndexVector.empty())
    edgeToTopOutlineIndexVector.clear();

  if (!edgeToBottomOulineIndexVector.empty())
    edgeToBottomOulineIndexVector.clear();

  quadsIndexArray.clear();
  quadsIndexCountArray.clear();
  quadsTopOutlineIndexArray.clear();
  quadsBottomOutlineIndexArray.clear();

  if (!edgeToPointIndexVector.empty())
    edgeToPointIndexVector.clear();

  if (!nodeToPointIndexVector.empty())
    nodeToPointIndexVector.clear();

  vectorLayoutSizeInit=false;
  vectorIndexSizeInit=false;
}

void GlVertexArrayManager::clearColorData() {
  toComputeColor=true;
  colorsUploadNeeded=true;

  linesColorsArray.clear();
  pointsColorsArray.clear();
  quadsColorsArray.clear();
  quadsOutlineColorsArray.clear();

  vectorColorSizeInit=false;
}

void GlVertexArrayManager::clearData() {
  toComputeAll=true;
  clearLayoutData();
  clearColorData();
}

void GlVertexArrayManager::initObservers() {
  if(!graph)
    return;

  if(!graphObserverActivated) {
    graph->addListener(this);
    graphObserverActivated=true;
  }

  if(!layoutObserverActivated) {
    layoutProperty->addListener(this);
    sizeProperty->addListener(this);
    shapeProperty->addListener(this);
    rotationProperty->addListener(this);
    srcAnchorShapeProperty->addListener(this);
    tgtAnchorShapeProperty->addListener(this);
    srcAnchorSizeProperty->addListener(this);
    tgtAnchorSizeProperty->addListener(this);
    layoutObserverActivated=true;
  }

  if(!colorObserverActivated) {
    colorProperty->addListener(this);
    borderColorProperty->addListener(this);
    borderWidthProperty->addListener(this);
    colorObserverActivated=true;
  }
}

void GlVertexArrayManager::clearObservers(PropertyInterface *deletedProperty) {

  if(graphObserverActivated) {
    graph->removeListener(this);
    graphObserverActivated=false;
  }

  if(layoutObserverActivated) {
    if(deletedProperty!=layoutProperty)
      layoutProperty->removeListener(this);

    if(deletedProperty!=sizeProperty)
      sizeProperty->removeListener(this);

    if(deletedProperty!=shapeProperty)
      shapeProperty->removeListener(this);

    if(deletedProperty!=rotationProperty)
      rotationProperty->removeListener(this);

    if(deletedProperty!=srcAnchorShapeProperty)
      srcAnchorShapeProperty->removeListener(this);

    if(deletedProperty!=tgtAnchorShapeProperty)
      tgtAnchorShapeProperty->removeListener(this);

    if(deletedProperty!=srcAnchorSizeProperty)
      srcAnchorSizeProperty->removeListener(this);

    if(deletedProperty!=tgtAnchorSizeProperty)
      tgtAnchorSizeProperty->removeListener(this);

    layoutObserverActivated=false;
  }

  if(colorObserverActivated) {
    if(deletedProperty!=colorProperty)
      colorProperty->removeListener(this);

    if(deletedProperty!=borderColorProperty)
      borderColorProperty->removeListener(this);

    if(deletedProperty!=borderWidthProperty)
      borderWidthProperty->removeListener(this);

    colorObserverActivated=false;
  }
}

void GlVertexArrayManager::treatEvent(const Event &evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_REVERSE_EDGE:
    case GraphEvent::TLP_AFTER_SET_ENDS:
      clearData();
      clearObservers();
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      const PropertyInterface *property=graph->getProperty(graphEvent->getPropertyName());

      if(property==colorProperty) { //Color property changed
        colorProperty = NULL;
        clearColorData();
      }
      else if(property==layoutProperty) {
        layoutProperty = NULL;
        clearLayoutData();
      }
      else if(property==sizeProperty) {
        sizeProperty = NULL;
        clearLayoutData();
      }
      else if(property==shapeProperty) {
        shapeProperty = NULL;
        clearLayoutData();
      }
      else if(property==rotationProperty) {
        rotationProperty = NULL;
        clearLayoutData();
      }
      else if(property==borderColorProperty) {
        borderColorProperty = NULL;
        clearColorData();
      }
      else if(property==borderWidthProperty) {
        borderWidthProperty = NULL;
        clearColorData();
      }
      else if(property==srcAnchorShapeProperty) {
        srcAnchorShapeProperty = NULL;
        clearLayoutData();
      }
      else if(property==tgtAnchorShapeProperty) {
        tgtAnchorShapeProperty = NULL;
        clearLayoutData();
      }
      else if(property==srcAnchorSizeProperty) {
        srcAnchorSizeProperty = NULL;
        clearLayoutData();
      }
      else if(property==tgtAnchorSizeProperty) {
        tgtAnchorSizeProperty = NULL;
        clearLayoutData();
      }

      break;
    }

    default:
      break;
    }
  }
  else if(evt.type() == Event::TLP_DELETE) {
    PropertyInterface* property=NULL;
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&evt);

    if(propertyEvent)
      property = propertyEvent->getProperty();

    clearData();
    clearObservers(property);
  }
  else {
    const PropertyEvent* propertyEvent = dynamic_cast<const PropertyEvent*>(&evt);
    PropertyInterface* property = propertyEvent->getProperty();

    switch(propertyEvent->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
      propertyValueChanged(property);
      break;

    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:

      if (layoutProperty==property || shapeProperty==property || srcAnchorShapeProperty==property || tgtAnchorShapeProperty==property ||
          srcAnchorSizeProperty==property || tgtAnchorSizeProperty==property) {
        edgesModified = true;
      }

      propertyValueChanged(property);
      break;

    default:
      break;
    }
  }
}

}
