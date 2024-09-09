/**
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

#include <GL/glew.h>

#include <tulip/OpenGlConfigManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlEdge.h>
#include <tulip/GlNode.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Curves.h>
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
GlVertexArrayManager::GlVertexArrayManager(GlGraphInputData *i)
    : inputData(i), graph(inputData->getGraph()), layoutProperty(inputData->getElementLayout()),
      sizeProperty(inputData->getElementSize()), shapeProperty(inputData->getElementShape()),
      rotationProperty(inputData->getElementRotation()),
      lengthRatioProperty(inputData->getEdgeLengthRatio()),
      colorProperty(inputData->getElementColor()),
      borderColorProperty(inputData->getElementBorderColor()),
      borderWidthProperty(inputData->getElementBorderWidth()),
      srcAnchorShapeProperty(inputData->getElementSrcAnchorShape()),
      tgtAnchorShapeProperty(inputData->getElementTgtAnchorShape()),
      srcAnchorSizeProperty(inputData->getElementSrcAnchorSize()),
      tgtAnchorSizeProperty(inputData->getElementTgtAnchorSize()), graphObserverActivated(false),
      layoutObserverActivated(false), colorObserverActivated(false), activated(true),
      isBegin(false), toComputeAll(true), toComputeLayout(true), toComputeColor(true),
      vectorLayoutSizeInit(false), vectorColorSizeInit(false), edgesModified(false),
      colorInterpolate(inputData->parameters->isEdgeColorInterpolate()),
      sizeInterpolate(inputData->parameters->isEdgeSizeInterpolate()),
      viewArrow(inputData->parameters->isViewArrow()), pointsVerticesVBO(0), pointsColorsVBO(0),
      linesVerticesVBO(0), linesColorsVBO(0), quadsVerticesVBO(0), quadsColorsVBO(0),
      quadsOutlineColorsVBO(0), pointsVerticesUploaded(false), pointsColorsUploaded(false),
      linesVerticesUploaded(false), linesColorsUploaded(false), quadsVerticesUploaded(false),
      quadsColorsUploaded(false), quadsOutlineColorsUploaded(false), verticesUploadNeeded(true),
      colorsUploadNeeded(true) {
  threadSafe = true;
}

GlVertexArrayManager::~GlVertexArrayManager() {
  clearObservers();
  clearData();
  bool canUseVBO = OpenGlConfigManager::hasVertexBufferObject();

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
  this->inputData = inputData;
  // Update properties with new ones
  layoutProperty = inputData->getElementLayout();
  sizeProperty = inputData->getElementSize();
  shapeProperty = inputData->getElementShape();
  rotationProperty = inputData->getElementRotation();
  lengthRatioProperty = inputData->getEdgeLengthRatio();
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
  graph = inputData->getGraph();
  initObservers();
}

bool GlVertexArrayManager::haveToCompute() {
  bool recompute = false;

  if (toComputeAll || toComputeLayout || toComputeColor) {
    recompute = true;
  }

  if (inputData->parameters->isEdgeColorInterpolate() != colorInterpolate) {
    colorInterpolate = inputData->parameters->isEdgeColorInterpolate();
    clearColorData();
    recompute = true;
  }

  if (inputData->parameters->isEdgeSizeInterpolate() != sizeInterpolate) {
    sizeInterpolate = inputData->parameters->isEdgeSizeInterpolate();
    clearLayoutData();
    recompute = true;
  }

  if (inputData->parameters->isViewArrow() != viewArrow) {
    viewArrow = inputData->parameters->isViewArrow();
    clearLayoutData();
    recompute = true;
  }

  // The layout property in the input data has changed => need to recompute layout.
  if (layoutProperty != inputData->getElementLayout()) {
    if (layoutProperty && layoutObserverActivated)
      layoutProperty->removeListener(this);

    layoutProperty = inputData->getElementLayout();
    layoutProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  // Size property changed
  if (sizeProperty != inputData->getElementSize()) {
    if (sizeProperty && layoutObserverActivated)
      sizeProperty->removeListener(this);

    sizeProperty = inputData->getElementSize();
    sizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  // Shape property changed
  if (shapeProperty != inputData->getElementShape()) {
    if (shapeProperty && layoutObserverActivated)
      shapeProperty->removeListener(this);

    shapeProperty = inputData->getElementShape();
    shapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  // Rotation property changed
  if (rotationProperty != inputData->getElementRotation()) {
    if (rotationProperty && layoutObserverActivated)
      rotationProperty->removeListener(this);

    rotationProperty = inputData->getElementRotation();
    rotationProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  // length ratio property changed
  if (lengthRatioProperty != inputData->getEdgeLengthRatio()) {
    if (lengthRatioProperty && layoutObserverActivated)
      lengthRatioProperty->removeListener(this);

    lengthRatioProperty = inputData->getEdgeLengthRatio();
    lengthRatioProperty->addListener(this);

    clearLayoutData();
    recompute = true;
  }

  // Color property changed
  if (colorProperty != inputData->getElementColor()) {
    if (colorProperty && colorObserverActivated)
      colorProperty->removeListener(this);

    colorProperty = inputData->getElementColor();
    colorProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  // Color property changed
  if (borderColorProperty != inputData->getElementBorderColor()) {
    if (borderColorProperty && colorObserverActivated)
      borderColorProperty->removeListener(this);

    borderColorProperty = inputData->getElementBorderColor();
    borderColorProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  // Border width property changed
  if (borderWidthProperty != inputData->getElementBorderWidth()) {
    if (borderWidthProperty && colorObserverActivated)
      borderWidthProperty->removeListener(this);

    borderWidthProperty = inputData->getElementBorderWidth();
    borderWidthProperty->addListener(this);
    clearColorData();
    recompute = true;
  }

  if (srcAnchorShapeProperty != inputData->getElementSrcAnchorShape()) {
    if (srcAnchorShapeProperty && layoutObserverActivated)
      srcAnchorShapeProperty->removeListener(this);

    srcAnchorShapeProperty = inputData->getElementSrcAnchorShape();
    srcAnchorShapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if (tgtAnchorShapeProperty != inputData->getElementTgtAnchorShape()) {
    if (tgtAnchorShapeProperty && layoutObserverActivated)
      tgtAnchorShapeProperty->removeListener(this);

    tgtAnchorShapeProperty = inputData->getElementTgtAnchorShape();
    tgtAnchorShapeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if (srcAnchorSizeProperty != inputData->getElementSrcAnchorSize()) {
    if (srcAnchorSizeProperty && layoutObserverActivated)
      srcAnchorSizeProperty->removeListener(this);

    srcAnchorSizeProperty = inputData->getElementSrcAnchorSize();
    srcAnchorSizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  if (tgtAnchorSizeProperty != inputData->getElementTgtAnchorSize()) {
    if (tgtAnchorSizeProperty && layoutObserverActivated)
      tgtAnchorSizeProperty->removeListener(this);

    tgtAnchorSizeProperty = inputData->getElementTgtAnchorSize();
    tgtAnchorSizeProperty->addListener(this);
    clearLayoutData();
    recompute = true;
  }

  return recompute;
}

void GlVertexArrayManager::setHaveToComputeAll(bool compute) {
  if (compute) {
    clearObservers();
    clearData();
  }

  if (!compute)
    initObservers();

  toComputeAll = compute;
  toComputeLayout = compute;
  toComputeColor = compute;
}

void GlVertexArrayManager::setHaveToComputeLayout(bool compute) {
  toComputeLayout = compute;
}

void GlVertexArrayManager::setHaveToComputeColor(bool compute) {
  toComputeColor = compute;
}

// we define a specific templated function to resize a vector<T>
// in order to avoid numerous unneeded calls to T default constructor
template <typename T>
inline void vector_set_size(std::vector<T> &v, unsigned int sz) {
  assert(v.capacity() >= sz);
  reinterpret_cast<T **>(&v)[1] = reinterpret_cast<T **>(&v)[0] + sz;
}

void GlVertexArrayManager::reserveMemoryForGraphElts(unsigned int nbNodes, unsigned int nbEdges) {
  auto nbSelectedNodes =
      inputData->getElementSelected()->numberOfNonDefaultValuatedNodes(inputData->getGraph());
  pointsNodesRenderingIndexArray.reserve(nbNodes - nbSelectedNodes);
  pointsNodesSelectedRenderingIndexArray.reserve(nbSelectedNodes);
  auto nbSelectedEdges =
      inputData->getElementSelected()->numberOfNonDefaultValuatedEdges(inputData->getGraph());
  pointsEdgesRenderingIndexArray.reserve(nbEdges - nbSelectedEdges);
  pointsEdgesSelectedRenderingIndexArray.reserve(nbSelectedEdges);

  if (!vectorLayoutSizeInit) {
    linesCoordsArray.reserve(2 * nbEdges);
    quadsCoordsArray.reserve(4 * nbEdges);
    pointsCoordsArray.reserve(nbNodes + nbEdges);
    // no need to initialize each Coord element
    vector_set_size<Coord>(pointsCoordsArray, nbNodes + nbEdges);
    edgeInfosVector.resize(nbEdges);

    vectorLayoutSizeInit = true;
  }

  if (!vectorColorSizeInit) {
    linesColorsArray.reserve(2 * nbEdges);
    quadsColorsArray.reserve(4 * nbEdges);
    pointsColorsArray.reserve(nbNodes + nbEdges);
    // no need to initialize each Color element
    vector_set_size<Color>(pointsColorsArray, nbNodes + nbEdges);

    vectorColorSizeInit = true;
  }
}

void GlVertexArrayManager::beginRendering() {
  if (!activated)
    return;

  isBegin = true;

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
}

void GlVertexArrayManager::endRendering() {
  if (!isBegin)
    return;

  isBegin = false;

  static bool canUseVBO = OpenGlConfigManager::hasVertexBufferObject();

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
    if (!pointsCoordsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glBufferData(GL_ARRAY_BUFFER, pointsCoordsArray.size() * 3 * sizeof(float),
                   VECTOR_DATA(pointsCoordsArray), GL_STATIC_DRAW);
      pointsVerticesUploaded = !isOpenGlOutOfMemory();
    }

    if (!linesCoordsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, linesVerticesVBO);
      glBufferData(GL_ARRAY_BUFFER, linesCoordsArray.size() * 3 * sizeof(float),
                   VECTOR_DATA(linesCoordsArray), GL_STATIC_DRAW);
      linesVerticesUploaded = !isOpenGlOutOfMemory();
    }

    if (!quadsCoordsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
      glBufferData(GL_ARRAY_BUFFER, quadsCoordsArray.size() * 3 * sizeof(float),
                   VECTOR_DATA(quadsCoordsArray), GL_STATIC_DRAW);
      quadsVerticesUploaded = !isOpenGlOutOfMemory();
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    verticesUploadNeeded = false;
  }

  if (canUseVBO && colorsUploadNeeded) {
    if (!pointsColorsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
      glBufferData(GL_ARRAY_BUFFER, pointsColorsArray.size() * 4 * sizeof(unsigned char),
                   VECTOR_DATA(pointsColorsArray), GL_STATIC_DRAW);
      pointsColorsUploaded = !isOpenGlOutOfMemory();
    }

    if (!linesColorsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, linesColorsVBO);
      glBufferData(GL_ARRAY_BUFFER, linesColorsArray.size() * 4 * sizeof(unsigned char),
                   VECTOR_DATA(linesColorsArray), GL_STATIC_DRAW);
      linesColorsUploaded = !isOpenGlOutOfMemory();
    }

    if (!quadsColorsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
      glBufferData(GL_ARRAY_BUFFER, quadsColorsArray.size() * 4 * sizeof(unsigned char),
                   VECTOR_DATA(quadsColorsArray), GL_STATIC_DRAW);
      quadsColorsUploaded = !isOpenGlOutOfMemory();
    }

    if (!quadsOutlineColorsArray.empty()) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsOutlineColorsVBO);
      glBufferData(GL_ARRAY_BUFFER, quadsOutlineColorsArray.size() * 4 * sizeof(unsigned char),
                   VECTOR_DATA(quadsOutlineColorsArray), GL_STATIC_DRAW);
      quadsOutlineColorsUploaded = !isOpenGlOutOfMemory();
    }

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
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    if (canUseVBO && pointsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    } else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
    }

    glDrawElements(GL_POINTS, pointsEdgesRenderingIndexArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(pointsEdgesRenderingIndexArray));

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
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    if (canUseVBO && pointsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    } else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(pointsColorsArray));
    }

    glDrawElements(GL_POINTS, pointsNodesRenderingIndexArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(pointsNodesRenderingIndexArray));

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
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
    }

    if (canUseVBO && linesColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, linesColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    } else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(linesColorsArray));
    }

    glDrawElements(GL_LINES, linesRenderingIndicesArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(linesRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Edges quad rendering
  if (!quadsRenderingIndicesArray.empty()) {
    if (canUseVBO && quadsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
    }

    if (!inputData->parameters->isEdgeColorInterpolate()) {
      if (canUseVBO && quadsOutlineColorsUploaded) {
        glBindBuffer(GL_ARRAY_BUFFER, quadsOutlineColorsVBO);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
      } else {
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsOutlineColorsArray));
      }
    } else {
      if (canUseVBO && quadsColorsUploaded) {
        glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
      } else {
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
      }
    }

    for (auto it = quadsOutlineRenderingIndicesArray.cbegin();
         it != quadsOutlineRenderingIndicesArray.cend(); ++it) {
      glLineWidth(it->first);
      glDrawElements(GL_LINES, it->second.size(), GL_UNSIGNED_INT, VECTOR_DATA(it->second));
    }

    if (canUseVBO && quadsColorsUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsColorsVBO);
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, BUFFER_OFFSET(0));
    } else {
      glColorPointer(4, GL_UNSIGNED_BYTE, 0, VECTOR_DATA(quadsColorsArray));
    }

    glDrawElements(GL_TRIANGLES, quadsRenderingIndicesArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(quadsRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  glDisableClientState(GL_COLOR_ARRAY);

  //============ Selected graph elements rendering ============================

  Color selectionColor = inputData->parameters->getSelectionColor();
  glColor4ubv(reinterpret_cast<const GLubyte *>(&selectionColor));

  // Selected edges point rendering
  glStencilFunc(GL_LEQUAL, inputData->parameters->getSelectedEdgesStencil(), 0xFFFF);
  glPointSize(2);

  if (!pointsEdgesSelectedRenderingIndexArray.empty()) {
    if (canUseVBO && pointsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, pointsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    glDrawElements(GL_POINTS, pointsEdgesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(pointsEdgesSelectedRenderingIndexArray));

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
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(pointsCoordsArray));
    }

    glDrawElements(GL_POINTS, pointsNodesSelectedRenderingIndexArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(pointsNodesSelectedRenderingIndexArray));

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
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(linesCoordsArray));
    }

    glDrawElements(GL_LINES, linesSelectedRenderingIndicesArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(linesSelectedRenderingIndicesArray));

    if (canUseVBO) {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
  }

  // Selected edges quad rendering
  if (!quadsSelectedRenderingIndicesArray.empty()) {
    if (canUseVBO && quadsVerticesUploaded) {
      glBindBuffer(GL_ARRAY_BUFFER, quadsVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
    } else {
      glVertexPointer(3, GL_FLOAT, 0, VECTOR_DATA(quadsCoordsArray));
    }

    glDrawElements(GL_TRIANGLES, quadsSelectedRenderingIndicesArray.size(), GL_UNSIGNED_INT,
                   VECTOR_DATA(quadsSelectedRenderingIndicesArray));

    for (auto it = quadsSelectedOutlineRenderingIndicesArray.begin();
         it != quadsSelectedOutlineRenderingIndicesArray.end(); ++it) {
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
  isBegin = !pause;
}

void GlVertexArrayManager::activate(bool act) {
  activated = act;
}

void GlVertexArrayManager::visit(GlEdge *glEdge) {
  edge e(glEdge->id);
  auto ends = graph->ends(e);
  node src = ends.first;
  node tgt = ends.second;

  auto nbNodes = graph->numberOfNodes();
  auto &eInfos = edgeInfosVector[glEdge->pos];

  if (toComputeLayout) {
    Coord srcCoord, tgtCoord;
    Size srcSize, tgtSize;

    const vector<Coord> &bends = layoutProperty->getEdgeValue(e);
    float lengthRatio = bends.size() ? 1 : lengthRatioProperty->getEdgeValue(e);
    vector<Coord> &vertices = eInfos.lineVertices;
    const unsigned int nbLines = glEdge->getVertices(inputData, e, src, tgt, srcCoord, tgtCoord,
                                                     srcSize, tgtSize, vertices, lengthRatio);

    if (nbLines != 0) {
      pointsCoordsArray[glEdge->pos + nbNodes] = vertices[0];

      Size edgeSize;
      float maxSrcSize, maxTgtSize;

      maxSrcSize = std::max(srcSize[0], srcSize[1]);
      maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

      glEdge->getEdgeSize(inputData, e, srcSize, tgtSize, maxSrcSize, maxTgtSize, edgeSize);

      vector<float> edgeSizes;
      getSizes(vertices, edgeSize[0] / 2.0f, edgeSize[1] / 2.0f, edgeSizes);

      vector<Coord> &quadVertices = eInfos.quadVertices;
      buildCurvePoints(vertices, edgeSizes, srcCoord, tgtCoord, quadVertices);

      glEdge->getEdgeAnchor(inputData, src, tgt, bends, srcCoord, tgtCoord, srcSize, tgtSize,
                            vertices[0], vertices[nbLines - 1], lengthRatio);
    }
  }

  if (toComputeColor) {
    const unsigned int nbLines = eInfos.lineVertices.size();

    if (nbLines != 0) {
      const Color &edgeColor = colorProperty->getEdgeValue(e);
      eInfos.edgeColor = edgeColor;
      eInfos.borderColor = borderColorProperty->getEdgeValue(e);
      Color srcColor, tgtColor;

      vector<Color> &lColors = eInfos.lineColors;
      glEdge->getColors(inputData, src, tgt, edgeColor, srcColor, tgtColor, &eInfos.lineVertices[0],
                        nbLines, lColors);
      pointsColorsArray[glEdge->pos + nbNodes] = lColors[0];

      const unsigned int nbQuads = eInfos.quadVertices.size();
      auto &quadVertices = eInfos.quadVertices;

      vector<Coord> centerLine;
      centerLine.reserve(nbQuads / 2);

      for (unsigned int i = 0; i < nbQuads / 2; ++i) {
        centerLine.push_back((quadVertices[2 * i] + quadVertices[2 * i + 1]) / 2.f);
      }

      vector<Color> &qColors = eInfos.quadColors;
      getColors(&centerLine[0], centerLine.size(), srcColor, tgtColor, qColors);
    }
  }
}

void GlVertexArrayManager::visit(GlNode *glNode) {
  // the first elts of pointsCoordsArray and pointsColorsArray
  // are dedicated to graph->nodes
  if (toComputeLayout) {
    pointsCoordsArray[glNode->pos] = glNode->getPoint(inputData);
  }

  if (toComputeColor) {
    pointsColorsArray[glNode->pos] = glNode->getColor(inputData);
  }
}

void GlVertexArrayManager::endOfVisit() {
  // we now have to build the global vectors
  // with the infos collected during the visit of edges
  for (auto &eInfos : edgeInfosVector) {
    auto &vertices = eInfos.lineVertices;
    if (vertices.empty())
      continue;
    if (toComputeLayout) {
      // update lines global vectors
      eInfos.linesIndex = linesCoordsArray.size();
      linesCoordsArray.insert(linesCoordsArray.end(), vertices.begin(), vertices.end());
      // update quads global vectors
      auto &quadVertices = eInfos.quadVertices;
      eInfos.quadsIndex = quadsCoordsArray.size();
      quadsCoordsArray.insert(quadsCoordsArray.end(), quadVertices.begin(), quadVertices.end());
    }
    if (toComputeColor) {
      auto &lcolors = eInfos.lineColors;
      linesColorsArray.insert(linesColorsArray.end(), lcolors.begin(), lcolors.end());

      auto &qcolors = eInfos.quadColors;
      if (colorInterpolate)
        for (size_t i = 0; i < qcolors.size(); ++i) {
          quadsColorsArray.push_back(qcolors[i]);
          quadsColorsArray.push_back(qcolors[i]);
        }
      else
        quadsColorsArray.insert(quadsColorsArray.end(), 2 * qcolors.size(), eInfos.edgeColor);
      quadsOutlineColorsArray.insert(quadsOutlineColorsArray.end(), 2 * qcolors.size(),
                                     eInfos.borderColor);
    }
  }
}

void GlVertexArrayManager::activateLineEdgeDisplay(GlEdge *glEdge, bool selected) {
  const unsigned int ePos = glEdge->pos;
  assert(ePos == graph->edgePos(edge(glEdge->id)));

  const edgeInfos &eInfos = edgeInfosVector[ePos];
  const unsigned int nbLines = eInfos.lineVertices.size();

  if (nbLines == 0)
    return;

  const unsigned int beginIndex = eInfos.linesIndex;
  const unsigned int endIndex = beginIndex + nbLines - 1;

  auto &renderingIndicesArray =
      selected ? linesSelectedRenderingIndicesArray : linesRenderingIndicesArray;
  for (unsigned int i = beginIndex; i < endIndex; ++i) {
    renderingIndicesArray.push_back(i);
    renderingIndicesArray.push_back(i + 1);
  }
}

void GlVertexArrayManager::activateQuadEdgeDisplay(GlEdge *glEdge, bool selected) {
  const unsigned int ePos = glEdge->pos;
  edge e(glEdge->id);
  assert(ePos == graph->edgePos(e));

  edgeInfos &eInfos = edgeInfosVector[ePos];
  const unsigned int nbQuads = eInfos.quadVertices.size();

  if (nbQuads == 0)
    return;

  const unsigned int beginIndex = eInfos.quadsIndex;
  const unsigned int endIndex = beginIndex + nbQuads - 2;

  auto &renderingIndicesArray =
      selected ? quadsSelectedRenderingIndicesArray : quadsRenderingIndicesArray;

  renderingIndicesArray.reserve(renderingIndicesArray.size() + 6 * ((endIndex - beginIndex) / 2));
  for (unsigned int i = beginIndex; i < endIndex; i += 2) {
    renderingIndicesArray.push_back(i);
    renderingIndicesArray.push_back(i + 1);
    renderingIndicesArray.push_back(i + 2);

    renderingIndicesArray.push_back(i + 2);
    renderingIndicesArray.push_back(i + 1);
    renderingIndicesArray.push_back(i + 3);
  }

  float borderWidth = float(inputData->getElementBorderWidth()->getEdgeValue(e));

  if (borderWidth > 0) {
    auto &outlineRenderingIndicesArray =
        selected ? quadsSelectedOutlineRenderingIndicesArray : quadsOutlineRenderingIndicesArray;

    if (outlineRenderingIndicesArray.find(borderWidth) == outlineRenderingIndicesArray.end()) {
      outlineRenderingIndicesArray[borderWidth] = vector<GLuint>();
    }

    auto &outlineRenderingIndices = outlineRenderingIndicesArray[borderWidth];
    outlineRenderingIndices.reserve(outlineRenderingIndices.size() + 4 * ((nbQuads / 2) - 1));
    for (unsigned int i = 0; i < (nbQuads / 2) - 1; ++i) {
      outlineRenderingIndices.push_back(beginIndex + 2 * i);
      outlineRenderingIndices.push_back(beginIndex + 2 * (i + 1));
    }

    for (unsigned int i = 0; i < (nbQuads / 2) - 1; ++i) {
      outlineRenderingIndices.push_back(beginIndex + 2 * i + 1);
      outlineRenderingIndices.push_back(beginIndex + 2 * (i + 1) + 1);
    }
  }
}

void GlVertexArrayManager::activatePointEdgeDisplay(GlEdge *glEdge, bool selected) {
  unsigned int ePos = glEdge->pos;
  assert(ePos == graph->edgePos(edge(glEdge->id)));

  if (edgeInfosVector[ePos].lineVertices.empty())
    return;

  ePos += graph->numberOfNodes();

  if (!selected) {
    pointsEdgesRenderingIndexArray.push_back(ePos);
  } else {
    pointsEdgesSelectedRenderingIndexArray.push_back(ePos);
  }
}

void GlVertexArrayManager::activatePointNodeDisplay(GlNode *glNode, bool selected) {
  const unsigned int index = glNode->pos;
  assert(index == graph->nodePos(tlp::node(glNode->id)));

  if (!selected) {
    pointsNodesRenderingIndexArray.push_back(index);
  } else {
    pointsNodesSelectedRenderingIndexArray.push_back(index);
  }
}

void GlVertexArrayManager::propertyValueChanged(PropertyInterface *property) {
  if (layoutProperty == property || sizeProperty == property || shapeProperty == property ||
      rotationProperty == property || lengthRatioProperty == property ||
      srcAnchorShapeProperty == property || tgtAnchorShapeProperty == property ||
      srcAnchorSizeProperty == property || tgtAnchorSizeProperty == property) {
    setHaveToComputeLayout(true);
    clearLayoutData();

    if (layoutProperty)
      layoutProperty->removeListener(this);

    if (sizeProperty)
      sizeProperty->removeListener(this);

    if (shapeProperty)
      shapeProperty->removeListener(this);

    if (rotationProperty)
      rotationProperty->removeListener(this);

    if (lengthRatioProperty)
      lengthRatioProperty->removeListener(this);

    layoutObserverActivated = false;
  }

  if (edgesModified || layoutProperty == property || lengthRatioProperty == property ||
      colorProperty == property || borderColorProperty == property ||
      borderWidthProperty == property) {
    setHaveToComputeColor(true);
    clearColorData();

    if (colorProperty)
      colorProperty->removeListener(this);

    if (borderColorProperty)
      borderColorProperty->removeListener(this);

    colorObserverActivated = false;
  }

  edgesModified = false;
}

void GlVertexArrayManager::clearLayoutData() {
  toComputeLayout = true;
  verticesUploadNeeded = true;

  linesCoordsArray.clear();
  pointsCoordsArray.clear();
  quadsCoordsArray.clear();

  edgeInfosVector.clear();

  vectorLayoutSizeInit = false;
}

void GlVertexArrayManager::clearColorData() {
  toComputeColor = true;
  colorsUploadNeeded = true;

  linesColorsArray.clear();
  pointsColorsArray.clear();
  quadsColorsArray.clear();
  quadsOutlineColorsArray.clear();

  vectorColorSizeInit = false;
}

void GlVertexArrayManager::clearData() {
  toComputeAll = true;
  clearLayoutData();
  clearColorData();
}

void GlVertexArrayManager::initObservers() {
  if (!graph)
    return;

  if (!graphObserverActivated) {
    graph->addListener(this);
    graphObserverActivated = true;
  }

  if (!layoutObserverActivated) {
    layoutProperty->addListener(this);
    sizeProperty->addListener(this);
    shapeProperty->addListener(this);
    rotationProperty->addListener(this);
    lengthRatioProperty->addListener(this);
    srcAnchorShapeProperty->addListener(this);
    tgtAnchorShapeProperty->addListener(this);
    srcAnchorSizeProperty->addListener(this);
    tgtAnchorSizeProperty->addListener(this);
    layoutObserverActivated = true;
  }

  if (!colorObserverActivated) {
    colorProperty->addListener(this);
    borderColorProperty->addListener(this);
    borderWidthProperty->addListener(this);
    colorObserverActivated = true;
  }
}

void GlVertexArrayManager::clearObservers(PropertyInterface *deletedProperty) {

  if (graphObserverActivated) {
    graph->removeListener(this);
    graphObserverActivated = false;
  }

  if (layoutObserverActivated) {
    if (deletedProperty != layoutProperty)
      layoutProperty->removeListener(this);

    if (deletedProperty != sizeProperty)
      sizeProperty->removeListener(this);

    if (deletedProperty != shapeProperty)
      shapeProperty->removeListener(this);

    if (deletedProperty != rotationProperty)
      rotationProperty->removeListener(this);

    if (lengthRatioProperty && (deletedProperty != lengthRatioProperty))
      lengthRatioProperty->removeListener(this);

    if (deletedProperty != srcAnchorShapeProperty)
      srcAnchorShapeProperty->removeListener(this);

    if (deletedProperty != tgtAnchorShapeProperty)
      tgtAnchorShapeProperty->removeListener(this);

    if (deletedProperty != srcAnchorSizeProperty)
      srcAnchorSizeProperty->removeListener(this);

    if (deletedProperty != tgtAnchorSizeProperty)
      tgtAnchorSizeProperty->removeListener(this);

    layoutObserverActivated = false;
  }

  if (colorObserverActivated) {
    if (deletedProperty != colorProperty)
      colorProperty->removeListener(this);

    if (deletedProperty != borderColorProperty)
      borderColorProperty->removeListener(this);

    if (deletedProperty != borderWidthProperty)
      borderWidthProperty->removeListener(this);

    colorObserverActivated = false;
  }
}

void GlVertexArrayManager::treatEvent(const Event &evt) {
  const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&evt);

  if (graphEvent) {

    switch (graphEvent->getType()) {
    case GraphEvent::TLP_ADD_NODE:
    case GraphEvent::TLP_ADD_EDGE:
    case GraphEvent::TLP_AFTER_DEL_NODE:
    case GraphEvent::TLP_AFTER_DEL_EDGE:
    case GraphEvent::TLP_REVERSE_EDGE:
    case GraphEvent::TLP_AFTER_SET_ENDS:
      clearData();
      clearObservers();
      break;

    case GraphEvent::TLP_BEFORE_DEL_INHERITED_PROPERTY:
    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY: {
      const PropertyInterface *property = graph->getProperty(graphEvent->getPropertyName());

      if (property == colorProperty) { // Color property changed
        colorProperty = nullptr;
        clearColorData();
      } else if (property == layoutProperty) {
        layoutProperty = nullptr;
        clearData();
      } else if (property == sizeProperty) {
        sizeProperty = nullptr;
        clearData();
      } else if (property == shapeProperty) {
        shapeProperty = nullptr;
        clearData();
      } else if (property == rotationProperty) {
        rotationProperty = nullptr;
        clearData();
      } else if (property == lengthRatioProperty) {
        lengthRatioProperty = nullptr;
        clearData();
      } else if (property == borderColorProperty) {
        borderColorProperty = nullptr;
        clearColorData();
      } else if (property == borderWidthProperty) {
        borderWidthProperty = nullptr;
        clearColorData();
      } else if (property == srcAnchorShapeProperty) {
        srcAnchorShapeProperty = nullptr;
        clearData();
      } else if (property == tgtAnchorShapeProperty) {
        tgtAnchorShapeProperty = nullptr;
        clearData();
      } else if (property == srcAnchorSizeProperty) {
        srcAnchorSizeProperty = nullptr;
        clearData();
      } else if (property == tgtAnchorSizeProperty) {
        tgtAnchorSizeProperty = nullptr;
        clearData();
      }

      break;
    }

    default:
      break;
    }
  } else if (evt.type() == Event::TLP_DELETE) {
    PropertyInterface *property = nullptr;
    const PropertyEvent *propertyEvent = dynamic_cast<const PropertyEvent *>(&evt);

    if (propertyEvent)
      property = propertyEvent->getProperty();

    clearData();
    clearObservers(property);
  } else {
    const PropertyEvent *propertyEvent = dynamic_cast<const PropertyEvent *>(&evt);
    PropertyInterface *property = propertyEvent->getProperty();

    switch (propertyEvent->getType()) {
    case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
      if (shapeProperty == property || sizeProperty == property) {
        edgesModified = true;
      }

      propertyValueChanged(property);
      break;

    case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
    case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:

      if (layoutProperty == property || shapeProperty == property ||
          lengthRatioProperty == property || srcAnchorShapeProperty == property ||
          tgtAnchorShapeProperty == property || srcAnchorSizeProperty == property ||
          tgtAnchorSizeProperty == property) {
        edgesModified = true;
      }

      propertyValueChanged(property);
      break;

    default:
      break;
    }
  }
}
} // namespace tlp
