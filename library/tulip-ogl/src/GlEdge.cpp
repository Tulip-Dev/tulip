/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <set>

#include <tulip/GlEdge.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlShaderProgram.h>
#include <tulip/GlTools.h>
#include <tulip/GlyphManager.h>
#include <tulip/Curves.h>
#include <tulip/GlGraphStaticData.h>
#include <tulip/GlLines.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Camera.h>
#include <tulip/GlBezierCurve.h>
#include <tulip/GlCatmullRomCurve.h>
#include <tulip/GlOpenUniformCubicBSpline.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/ParametricCurves.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlScene.h>
#include <tulip/GlGlyphRenderer.h>
#include <tulip/TulipViewSettings.h>

using namespace std;

namespace tlp {

BoundingBox GlEdge::getBoundingBox(const GlGraphInputData *data) {
  edge e = edge(id);

  auto ends = data->graph->ends(e);
  const node src = ends.first;
  const node tgt = ends.second;
  const Coord &srcCoord = data->getElementLayout()->getNodeValue(src);
  const Coord &tgtCoord = data->getElementLayout()->getNodeValue(tgt);

  const Size &srcSize = data->getElementSize()->getNodeValue(src);
  const Size &tgtSize = data->getElementSize()->getNodeValue(tgt);
  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);

  return getBoundingBox(data, e, src, tgt, srcCoord, tgtCoord, srcSize, tgtSize, bends);
}

BoundingBox GlEdge::getBoundingBox(const GlGraphInputData *data, const edge e, const node src,
                                   const node tgt, const Coord &srcCoord, const Coord &tgtCoord,
                                   const Size &srcSize, const Size &tgtSize,
                                   const LineType::RealType &bends) {

  double srcRot = data->getElementRotation()->getNodeValue(src);
  double tgtRot = data->getElementRotation()->getNodeValue(tgt);

  // set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
  Coord srcAnchor, tgtAnchor, tmpAnchor;
  bool hasBends(!bends.empty());
  int srcGlyphId = data->getElementShape()->getNodeValue(src);
  Glyph *srcGlyph = data->glyphs.get(srcGlyphId);
  tmpAnchor = hasBends ? bends.front() : tgtCoord;
  srcAnchor = srcGlyph->getAnchor(srcCoord, tmpAnchor, srcSize, srcRot);

  int tgtGlyphId = 1; // cube outlined

  if (!data->getGraph()->isMetaNode(tgt))
    tgtGlyphId = data->getElementShape()->getNodeValue(tgt);

  Glyph *tgtGlyph = data->glyphs.get(tgtGlyphId);
  // this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
  tmpAnchor = hasBends ? bends.back() : srcAnchor;
  tgtAnchor = tgtGlyph->getAnchor(tgtCoord, tmpAnchor, tgtSize, tgtRot);

  vector<Coord> tmp;
  tlp::computeCleanVertices(bends, srcCoord, tgtCoord, srcAnchor, tgtAnchor, tmp);

  BoundingBox bb(srcAnchor, tgtAnchor, true);
  if (!tmp.empty()) {

    Size edgeSize;
    float maxSrcSize, maxTgtSize;

    maxSrcSize = std::max(srcSize[0], srcSize[1]);
    maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

    getEdgeSize(data, e, srcSize, tgtSize, maxSrcSize, maxTgtSize, edgeSize);

    vector<float> edgeSizes;
    getSizes(tmp, edgeSize[0] / 2.0f, edgeSize[1] / 2.0f, edgeSizes);

    vector<Coord> quadVertices;
    buildCurvePoints(tmp, edgeSizes, srcCoord, tgtCoord, quadVertices);

    for (size_t i = 0; i < quadVertices.size(); ++i) {
      bb.expand(quadVertices[i], true);
    }
  }

  return bb;
}

void GlEdge::draw(float lod, const GlGraphInputData *data, Camera *camera) {

  edge e(id);

  auto ends = data->graph->ends(e);
  const node src = ends.first;
  const node tgt = ends.second;

  bool selected = data->getElementSelected()->getEdgeValue(e);

  Color srcCol, tgtCol;
  getEdgeColor(data, e, src, tgt, selected, srcCol, tgtCol);

  const Color &strokeColor = data->getElementBorderColor()->getEdgeValue(e);
  double borderWidth = data->getElementBorderWidth()->getEdgeValue(e);

  if (!selectionDraw && (srcCol.getA() == 0) && (tgtCol.getA() == 0) &&
      (borderWidth == 0 || strokeColor.getA() == 0)) {
    // edge is fully transparent, no need to continue the rendering process
    return;
  }

  const Size &srcSize = data->getElementSize()->getNodeValue(src);
  const Size &tgtSize = data->getElementSize()->getNodeValue(tgt);

  Size edgeSize;
  float maxSrcSize, maxTgtSize;

  maxSrcSize = std::max(srcSize[0], srcSize[1]);
  maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

  getEdgeSize(data, e, srcSize, tgtSize, maxSrcSize, maxTgtSize, edgeSize);

  const Coord &srcCoord = data->getElementLayout()->getNodeValue(src);

  float lodSize = getEdgeWidthLod(srcCoord, edgeSize, camera);

  if (lod < 5) {
    if (data->getGlVertexArrayManager()->renderingIsBegin()) {
      data->getGlVertexArrayManager()->activatePointEdgeDisplay(this, selected);
    } else {

      setColor(srcCol);
      glPointSize(1);
      glBegin(GL_POINTS);
      glVertex3f(srcCoord[0], srcCoord[1], srcCoord[2]);
      glEnd();
    }

    return;
  }

  const std::string &edgeTexture = data->getElementTexture()->getEdgeValue(e);
  bool vertexArrayRendering = false;

  if (data->getGlVertexArrayManager()->renderingIsBegin()) {
    if (lodSize > -5 && lodSize < 5) {
      data->getGlVertexArrayManager()->activateLineEdgeDisplay(this, selected);
      return;
    } else if (!data->parameters->isEdge3D() && edgeTexture.empty()) {
      vertexArrayRendering = true;
      data->getGlVertexArrayManager()->activateQuadEdgeDisplay(this, selected);
    }
  }

  const Coord &tgtCoord = data->getElementLayout()->getNodeValue(tgt);

  if (selected) {
    glStencilFunc(GL_LEQUAL, data->parameters->getSelectedEdgesStencil(), 0xFFFF);
  } else {
    glStencilFunc(GL_LEQUAL, data->parameters->getEdgesStencil(), 0xFFFF);
  }

  glEnable(GL_COLOR_MATERIAL);

  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  bool hasBends(!bends.empty());

  if (!hasBends && ((src == tgt) /* a loop without bends: draw a nice loop!! */
                    || (srcCoord - tgtCoord).norm() < 1E-4) /* two very close nodes */) {
    return;
  }

  Coord srcAnchor, tgtAnchor, beginLineAnchor, endLineAnchor;

  getEdgeAnchor(data, src, tgt, bends, srcCoord, tgtCoord, srcSize, tgtSize, srcAnchor, tgtAnchor);

  if (data->parameters->isViewArrow()) {
    EdgeExtremityGlyph *startEdgeGlyph =
        data->extremityGlyphs.get(data->getElementSrcAnchorShape()->getEdgeValue(e));
    EdgeExtremityGlyph *endEdgeGlyph =
        data->extremityGlyphs.get(data->getElementTgtAnchorShape()->getEdgeValue(e));

    float selectionOutlineSize = 0.f;

    if (selected) {
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      Coord &&p1 = camera->viewportTo3DWorld(Coord(0, 0, 0));
      Coord &&p2 = camera->viewportTo3DWorld(Coord(2, 0, 0));
      selectionOutlineSize = (p2 - p1).norm();
      edgeSize[0] += selectionOutlineSize;
      edgeSize[1] += selectionOutlineSize;

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }

    if (startEdgeGlyph != nullptr) {
      displayArrowAndAdjustAnchor(data, e, src, data->getElementSrcAnchorSize()->getEdgeValue(e),
                                  std::min(srcSize[0], srcSize[1]), srcCol, maxSrcSize, selected,
                                  selectionOutlineSize,
                                  endEdgeGlyph ? endEdgeGlyph->id() : UINT_MAX, hasBends,
                                  hasBends ? bends.front() : tgtCoord, tgtCoord, srcAnchor,
                                  tgtAnchor, beginLineAnchor, startEdgeGlyph, camera);
    } else {
      beginLineAnchor = srcAnchor;
    }

    if (endEdgeGlyph != nullptr) {
      displayArrowAndAdjustAnchor(data, e, tgt, data->getElementTgtAnchorSize()->getEdgeValue(e),
                                  std::min(tgtSize[0], tgtSize[1]), tgtCol, maxTgtSize, selected,
                                  selectionOutlineSize,
                                  startEdgeGlyph ? startEdgeGlyph->id() : UINT_MAX, hasBends,
                                  hasBends ? bends.back() : srcAnchor, srcCoord, tgtAnchor,
                                  srcAnchor, endLineAnchor, endEdgeGlyph, camera);
    } else {
      endLineAnchor = tgtAnchor;
    }
  } else {
    beginLineAnchor = srcAnchor;
    endLineAnchor = tgtAnchor;
  }

  if (vertexArrayRendering)
    return;

  // Reset in case of drawing extremity glyph that can alterate value
  GlTextureManager::setAnimationFrame(data->getElementAnimationFrame()->getEdgeValue(e));
  // draw Edge
  drawEdge(srcCoord, tgtCoord, beginLineAnchor, endLineAnchor, bends, srcCol, tgtCol,
           camera->getCenter() - camera->getEyes(), data->parameters->isEdgeColorInterpolate(),
           strokeColor, edgeSize, data->getElementShape()->getEdgeValue(e),
           data->parameters->isEdge3D(), lodSize, edgeTexture, borderWidth);
  GlTextureManager::setAnimationFrame(0);

  glEnable(GL_LIGHTING);
}

#define L3D_BIT (1 << 9)
void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos, const Coord &startPoint,
                      const Coord &endPoint, const LineType::RealType &bends,
                      const Color &startColor, const Color &endColor, const Coord &lookDir,
                      bool colorInterpolate, const Color &borderColor, const Size &size, int shape,
                      bool edge3D, float lod, const string &textureName, const float outlineWidth) {

  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);

  if (bends.empty())
    shape = EdgeShape::Polyline;

  Coord srcDir(srcNodePos);
  Coord tgtDir(tgtNodePos);
  vector<Coord> tmp;
  computeCleanVertices(bends, startPoint, endPoint, srcDir, tgtDir, tmp);

  if (tmp.size() < 2) {
    return;
  }

  if (edge3D) {
    shape |= L3D_BIT;
    glEnable(GL_LIGHTING);
  } else {
    glDisable(GL_LIGHTING);
  }

  switch (shape) {
  case EdgeShape::Polyline:

    if (lod > 1000 || lod < -1000) {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,
                    colorInterpolate, borderColor, textureName, outlineWidth);
    } else {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir, true,
                    borderColor, textureName, outlineWidth);
    }

    break;

  case L3D_BIT + EdgeShape::Polyline: {
    glDisable(GL_LIGHTING);
    simpleQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir, lookDir,
               colorInterpolate, borderColor, textureName, outlineWidth);
    glEnable(GL_LIGHTING);
    break;
  }

  case EdgeShape::BezierCurve:
  case EdgeShape::CatmullRomCurve:
  case EdgeShape::CubicBSplineCurve:
  case EdgeShape::BezierCurve + L3D_BIT:
  case EdgeShape::CatmullRomCurve + L3D_BIT:
  case EdgeShape::CubicBSplineCurve + L3D_BIT: {
    static GlBezierCurve bezier;
    static GlCatmullRomCurve catmull;
    static GlOpenUniformCubicBSpline bspline;
    AbstractGlCurve *curve = nullptr;
    unsigned int nbCurvePoints = 200;

    if (shape == EdgeShape::BezierCurve || shape == EdgeShape::BezierCurve + L3D_BIT) {
      curve = &bezier;
    } else if (shape == EdgeShape::CatmullRomCurve ||
               shape == EdgeShape::CatmullRomCurve + L3D_BIT) {
      curve = &catmull;
    } else {
      curve = &bspline;
    }

    bool fisheyeActivated = GlShaderProgram::getCurrentActiveShader() &&
                            GlShaderProgram::getCurrentActiveShader()->getName() == "fisheye";

    curve->setLineCurve(false);
    curve->setOutlined(false);
    curve->setOutlineColor(borderColor);
    curve->setOutlineColorInterpolation(colorInterpolate);
    curve->setBillboardCurve(false);
    curve->setTexture(textureName);

    if (edge3D) {
      curve->setBillboardCurve(true);
      curve->setLookDir(lookDir);
    }

    float startSize = size[0] * 0.5f, endSize = size[1] * 0.5f;

    if (!fisheyeActivated && lod > -5 && lod < 5) {
      curve->setLineCurve(true);
      curve->setCurveLineWidth(1.4f);
    }

    else if (fisheyeActivated || lod > 5 || lod < -5) {
      curve->setOutlined(outlineWidth > 0);
      curve->setCurveQuadBordersWidth(outlineWidth);
    }

    curve->drawCurve(tmp, startColor, endColor, startSize, endSize, nbCurvePoints);
    break;
  }

  default:

    if (lod > 1000 || lod < -1000) {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,
                    colorInterpolate, borderColor);
    } else {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir, true,
                    borderColor);
    }

    break;
  }

  glDepthFunc(GL_LEQUAL);
}

void GlEdge::drawLabel(GlLabel &label, OcclusionTest *test, const GlGraphInputData *data, float lod,
                       Camera *camera) {

  edge e(id);

  const string &tmp = data->getElementLabel()->getEdgeValue(e);

  if (tmp.empty())
    return;

  bool select = data->getElementSelected()->getEdgeValue(e);

  Color fontColor, outlineColor;

  if (select) {
    fontColor = outlineColor = data->parameters->getSelectionColor();
  } else {
    fontColor = data->getElementLabelColor()->getEdgeValue(e);
    outlineColor = data->getElementLabelBorderColor()->getEdgeValue(e);
  }

  float outlineWidth = data->getElementLabelBorderWidth()->getEdgeValue(e);

  if (fontColor.getA() == 0 && (outlineColor.getA() == 0 || outlineWidth == 0))
    return;

  int fontSize = data->getElementFontSize()->getEdgeValue(e);

  if (select) {
    fontSize += 2;
    label.setStencil(data->parameters->getSelectedEdgesStencil());
  } else
    label.setStencil(data->parameters->getEdgesLabelStencil());

  label.setFontNameSizeAndColor(data->getElementFont()->getEdgeValue(e), fontSize, fontColor);
  label.setText(tmp);
  label.setOutlineColor(outlineColor);
  label.setOutlineSize(outlineWidth);

  auto ends = data->graph->ends(e);
  const node src = ends.first;
  const node tgt = ends.second;

  const Size &srcSize = data->getElementSize()->getNodeValue(src);
  const Size &tgtSize = data->getElementSize()->getNodeValue(tgt);
  Size edgeSize;
  float maxSrcSize, maxTgtSize;

  maxSrcSize = std::max(srcSize[0], srcSize[1]);
  maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

  getEdgeSize(data, e, srcSize, tgtSize, maxSrcSize, maxTgtSize, edgeSize);

  label.setTranslationAfterRotation(Coord());

  const Coord &srcCoord = data->getElementLayout()->getNodeValue(src);
  const Coord &tgtCoord = data->getElementLayout()->getNodeValue(tgt);
  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  Coord position;
  float angle;

  if (bends.empty()) {
    position = (srcCoord + tgtCoord) / 2.f;
    angle = atan((tgtCoord[1] - srcCoord[1]) / (tgtCoord[0] - srcCoord[0])) * float(180. / M_PI);
  } else {
    if (bends.size() % 2 == 0) {
      position = (bends[bends.size() / 2 - 1] + bends[bends.size() / 2]) / 2.f;
      angle = atan((bends[bends.size() / 2][1] - bends[bends.size() / 2 - 1][1]) /
                   (bends[bends.size() / 2][0] - bends[bends.size() / 2 - 1][0])) *
              float(180. / M_PI);
    } else {
      position = bends[bends.size() / 2];
      Coord firstVector;
      Coord secondVector;

      if (bends.size() > 1) {
        firstVector = bends[bends.size() / 2] - bends[bends.size() / 2 - 1];
        secondVector = bends[bends.size() / 2] - bends[bends.size() / 2 + 1];
      } else {
        firstVector = bends[bends.size() / 2] - srcCoord;
        secondVector = bends[bends.size() / 2] - tgtCoord;
      }

      float firstAngle = atan(firstVector[1] / firstVector[0]) * float(180. / M_PI);
      float secondAngle = atan(secondVector[1] / secondVector[0]) * float(180. / M_PI);

      Coord &&textDirection = firstVector + secondVector;

      if (textDirection[1] < 0)
        label.setTranslationAfterRotation(Coord(0, -label.getTranslationAfterRotation()[1], 0));

      angle = (firstAngle + secondAngle) / 2.f;

      if (firstVector[0] * secondVector[0] >= 0)
        angle += 90;

      if (angle >= 90)
        angle = -180 + angle;
    }
  }

  int labelPos = data->getElementLabelPosition()->getEdgeValue(e);

  label.setSize(Size());
  label.rotate(0, 0, angle);
  label.setAlignment(labelPos);
  label.setScaleToSize(false);
  label.setLabelsDensity(data->parameters->getLabelsDensity());

  if (!(data->parameters->getLabelsDensity() == 100)) // labels overlap
    label.setOcclusionTester(test);
  else
    label.setOcclusionTester(nullptr);

  label.setPosition(position);

  if (edgeSize[0] > edgeSize[1]) {
    label.setTranslationAfterRotation(Coord(0, -edgeSize[0] / 2));
  } else {
    label.setTranslationAfterRotation(Coord(0, -edgeSize[1] / 2));
  }

  BoundingBox &&bb = getBoundingBox(data, e, src, tgt, srcCoord, tgtCoord, srcSize, tgtSize, bends);
  label.setUseLODOptimisation(true, bb);
  label.setUseMinMaxSize(!data->parameters->isLabelFixedFontSize());
  label.setMinSize(data->parameters->getMinSizeOfLabel());
  label.setMaxSize(data->parameters->getMaxSizeOfLabel());
  label.setBillboarded(data->parameters->getLabelsAreBillboarded());

  label.drawWithStencil(lod, camera);
}

size_t GlEdge::getVertices(const GlGraphInputData *data, const edge e, const node src,
                           const node tgt, Coord &srcCoord, Coord &tgtCoord, Size &srcSize,
                           Size &tgtSize, std::vector<Coord> &vertices) {
  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  bool hasBends(!bends.empty());

  if (!hasBends && (src == tgt)) { // a loop without bends
    return 0;
  }

  srcCoord = data->getElementLayout()->getNodeValue(src);
  tgtCoord = data->getElementLayout()->getNodeValue(tgt);
  if (!hasBends && (srcCoord - tgtCoord).norm() < 1E-4)
    return 0;

  srcSize = data->getElementSize()->getNodeValue(src);
  tgtSize = data->getElementSize()->getNodeValue(tgt);

  float maxSrcSize, maxTgtSize;

  maxSrcSize = std::max(srcSize[0], srcSize[1]);
  maxTgtSize = std::max(tgtSize[0], tgtSize[1]);

  Coord srcAnchor, tgtAnchor;
  getEdgeAnchor(data, src, tgt, bends, srcCoord, tgtCoord, srcSize, tgtSize, srcAnchor, tgtAnchor);

  EdgeExtremityGlyph *startEdgeGlyph =
      data->extremityGlyphs.get(data->getElementSrcAnchorShape()->getEdgeValue(e));
  EdgeExtremityGlyph *endEdgeGlyph =
      data->extremityGlyphs.get(data->getElementTgtAnchorShape()->getEdgeValue(e));

  Coord beginLineAnchor;
  bool selected = data->getElementSelected()->getEdgeValue(e);

  if (data->parameters->isViewArrow() && startEdgeGlyph != nullptr) {
    displayArrowAndAdjustAnchor(data, e, src, data->getElementSrcAnchorSize()->getEdgeValue(e),
                                std::min(srcSize[0], srcSize[1]), Color(), maxSrcSize, selected, 0,
                                endEdgeGlyph ? endEdgeGlyph->id() : UINT_MAX, hasBends,
                                hasBends ? bends.front() : tgtCoord, tgtCoord, srcAnchor, tgtAnchor,
                                beginLineAnchor);
  } else {
    beginLineAnchor = srcAnchor;
  }

  Coord endLineAnchor;

  if (data->parameters->isViewArrow() && endEdgeGlyph != nullptr) {
    displayArrowAndAdjustAnchor(data, e, tgt, data->getElementTgtAnchorSize()->getEdgeValue(e),
                                std::min(tgtSize[0], tgtSize[1]), Color(), maxTgtSize, selected, 0,
                                startEdgeGlyph ? startEdgeGlyph->id() : UINT_MAX, hasBends,
                                hasBends ? bends.back() : srcAnchor, srcCoord, tgtAnchor, srcAnchor,
                                endLineAnchor);
  } else {
    endLineAnchor = tgtAnchor;
  }

  computeCleanVertices(bends, beginLineAnchor, endLineAnchor, srcCoord, tgtCoord, vertices, false);

  if (vertices.empty())
    return 0;

  auto edgeShape = data->getElementShape()->getEdgeValue(e);
  auto vSize = vertices.size();
  if ((vSize > 2 && edgeShape == EdgeShape::BezierCurve) ||
      (vSize == 3 && edgeShape == EdgeShape::CubicBSplineCurve)) {
    vector<Coord> curvePoints;
    computeBezierPoints(vertices, curvePoints, 200);
    vertices.swap(curvePoints);
  } else if (vSize > 2 && edgeShape == EdgeShape::CatmullRomCurve) {
    vector<Coord> curvePoints;
    computeCatmullRomPoints(vertices, curvePoints, false, 200);
    vertices.swap(curvePoints);
  }

  if (vSize > 2 && edgeShape == EdgeShape::CubicBSplineCurve) {
    vector<Coord> curvePoints;
    computeOpenUniformBsplinePoints(vertices, curvePoints, 3, 200);
    vertices.swap(curvePoints);
  }

  return vertices.size();
}

void GlEdge::getColors(const GlGraphInputData *data, const node src, const node tgt,
                       const Color &eColor, Color &srcCol, Color &tgtCol, const Coord *vertices,
                       unsigned int numberOfVertices, std::vector<Color> &colors) {
  if (data->parameters->isEdgeColorInterpolate()) {
    srcCol = data->getElementColor()->getNodeValue(src);
    tgtCol = data->getElementColor()->getNodeValue(tgt);
  } else {
    srcCol = tgtCol = eColor;
  }

  tlp::getColors(vertices, numberOfVertices, srcCol, tgtCol, colors);
}

void GlEdge::getEdgeColor(const GlGraphInputData *data, const edge e, const node src,
                          const node tgt, bool selected, Color &srcCol, Color &tgtCol) {

  if (selected) {
    srcCol = tgtCol = data->parameters->getSelectionColor();
  } else {
    if (data->parameters->isEdgeColorInterpolate()) {
      srcCol = data->getElementColor()->getNodeValue(src);
      tgtCol = data->getElementColor()->getNodeValue(tgt);
    } else {
      srcCol = tgtCol = data->getElementColor()->getEdgeValue(e);
    }
  }
}

void GlEdge::getEdgeSize(const GlGraphInputData *data, edge e, const Size &srcSize,
                         const Size &tgtSize, const float maxSrcSize, const float maxTgtSize,
                         Size &edgeSize) {
  Size sz;
  if (data->parameters->isEdgeSizeInterpolate()) {
    sz[0] = std::min(srcSize[0], srcSize[1]) / 8.f;
    sz[1] = std::min(tgtSize[0], tgtSize[1]) / 8.f;
  } else {
    sz = data->getElementSize()->getEdgeValue(e);

    if (data->parameters->getEdgesMaxSizeToNodesSize()) {
      sz[0] = std::min(maxSrcSize, sz[0]);
      sz[1] = std::min(maxTgtSize, sz[1]);
    }

    sz[0] /= 2.f;
    sz[1] /= 2.f;
  }
  edgeSize = sz;
}

void GlEdge::getEdgeAnchor(const GlGraphInputData *data, const node src, const node tgt,
                           const LineType::RealType &bends, const Coord &srcCoord,
                           const Coord &tgtCoord, const Size &srcSize, const Size &tgtSize,
                           Coord &srcAnchor, Coord &tgtAnchor) {
  double srcRot = data->getElementRotation()->getNodeValue(src);
  double tgtRot = data->getElementRotation()->getNodeValue(tgt);

  // compute anchor, (clip line with the glyph)
  int srcGlyphId = data->getElementShape()->getNodeValue(src);
  Glyph *srcGlyph = data->glyphs.get(srcGlyphId);
  srcAnchor = (bends.size() > 0) ? bends.front() : tgtCoord;
  srcAnchor = srcGlyph->getAnchor(srcCoord, srcAnchor, srcSize, srcRot);

  // compute anchor, (clip line with the glyph)
  int tgtGlyphId = data->getElementShape()->getNodeValue(tgt);
  Glyph *tgtGlyph = data->glyphs.get(tgtGlyphId);
  tgtAnchor = (bends.size() > 0) ? bends.back() : srcAnchor;
  tgtAnchor = tgtGlyph->getAnchor(tgtCoord, tgtAnchor, tgtSize, tgtRot);
}

float GlEdge::getEdgeWidthLod(const Coord &edgeCoord, const Size &edgeSize, Camera *camera) {
  Matrix<float, 4u> projectionMatrix;
  Matrix<float, 4u> modelviewMatrix;
  camera->getProjectionMatrix(projectionMatrix);
  camera->getModelviewMatrix(modelviewMatrix);

  if (edgeSize[0] != edgeSize[1]) {
    return std::max(
        std::abs(projectSize(edgeCoord, Size(edgeSize[0], edgeSize[0], edgeSize[0]),
                             projectionMatrix, modelviewMatrix, camera->getViewport())),
        std::abs(projectSize(edgeCoord, Size(edgeSize[1], edgeSize[1], edgeSize[1]),
                             projectionMatrix, modelviewMatrix, camera->getViewport())));
  } else {
    return std::abs(projectSize(edgeCoord, Size(edgeSize[0], edgeSize[0], edgeSize[0]),
                                projectionMatrix, modelviewMatrix, camera->getViewport()));
  }
}

void GlEdge::displayArrowAndAdjustAnchor(const GlGraphInputData *data, const edge e, const node src,
                                         const Size &sizeRatio, float edgeSize, const Color &color,
                                         float maxSize, bool selected, float selectionOutlineSize,
                                         int tgtEdgeGlyph, bool hasBends, const Coord &anchor,
                                         const Coord &tgtCoord, const Coord &srcAnchor,
                                         const Coord &tgtAnchor, Coord &lineAnchor,
                                         EdgeExtremityGlyph *extremityGlyph, Camera *camera) {

  // Correct begin tmp Anchor
  Coord beginTmpAnchor = anchor;

  if (beginTmpAnchor == tgtCoord) {
    beginTmpAnchor = tgtAnchor;
  }

  lineAnchor = beginTmpAnchor - srcAnchor;
  float nrm = lineAnchor.norm();
  float maxGlyphSize;

  if (tgtEdgeGlyph != 0 && !hasBends)
    maxGlyphSize = nrm * .5f;
  else
    maxGlyphSize = nrm;

  Size size;

  if (data->parameters->isEdgeSizeInterpolate()) {
    size[0] = size[1] = size[2] = edgeSize / 4.0f;
  } else {
    size = sizeRatio[0];

    if (data->parameters->getEdgesMaxSizeToNodesSize()) {
      size[0] = std::min(maxSize, size[0]);
      size[1] = std::min(maxSize, size[1]);
      size[2] = std::min(maxSize, size[2]);
    }
  }

  if (selected) {
    size[1] += selectionOutlineSize;
    size[2] += selectionOutlineSize;
  }

  size[0] = std::min(maxGlyphSize, size[0]);

  if (extremityGlyph) {

    MatrixGL projectionMatrix;
    MatrixGL modelviewMatrix;
    camera->getProjectionMatrix(projectionMatrix);
    camera->getModelviewMatrix(modelviewMatrix);

    float lod =
        projectSize(srcAnchor, size, projectionMatrix, modelviewMatrix, camera->getViewport());

    // edge extremity glyph is in the viewport
    if (lod > 0) {

      // Some glyphs can not benefit from the shader rendering optimization
      // due to the use of quadrics or modelview matrix modification or lighting effect
      static set<int> noShaderGlyphs;

      if (noShaderGlyphs.empty()) {
        noShaderGlyphs.insert(EdgeExtremityShape::Cone);
        noShaderGlyphs.insert(EdgeExtremityShape::Cylinder);
        noShaderGlyphs.insert(EdgeExtremityShape::GlowSphere);
        noShaderGlyphs.insert(EdgeExtremityShape::Sphere);
        noShaderGlyphs.insert(EdgeExtremityShape::Cube);
        noShaderGlyphs.insert(EdgeExtremityShape::FontAwesomeIcon);
      }

      Color borderColor = data->parameters->isEdgeColorInterpolate()
                              ? color
                              : data->getElementBorderColor()->getEdgeValue(e);

      if (data->getGlGlyphRenderer()->renderingHasStarted() &&
          noShaderGlyphs.find(extremityGlyph->id()) == noShaderGlyphs.end()) {
        data->getGlGlyphRenderer()->addEdgeExtremityGlyphRendering(
            extremityGlyph, e, src, color, borderColor, 100., beginTmpAnchor, srcAnchor, size,
            selected);
      } else {
        MatrixGL srcTransformationMatrix;
        MatrixGL srcScalingMatrix;

        extremityGlyph->get2DTransformationMatrix(beginTmpAnchor, srcAnchor, size,
                                                  srcTransformationMatrix, srcScalingMatrix);

        glPushMatrix();
        glMultMatrixf(reinterpret_cast<GLfloat *>(&srcTransformationMatrix));
        glMultMatrixf(reinterpret_cast<GLfloat *>(&srcScalingMatrix));
        glDisable(GL_CULL_FACE);
        extremityGlyph->draw(e, src, color, borderColor, 100.);
        glEnable(GL_CULL_FACE);
        glPopMatrix();
      }
    }
  }

  // Compute new Anchor

  if (nrm > 0.00000001f) {
    lineAnchor /= nrm;
    lineAnchor *= size[0];
    lineAnchor += srcAnchor;
  } else {
    lineAnchor = srcAnchor;
  }
}
} // namespace tlp
