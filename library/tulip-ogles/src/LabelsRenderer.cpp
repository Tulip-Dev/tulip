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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <GL/glew.h>

#include <sstream>
#include <fstream>
#include <set>
#include <iostream>
#include <cmath>

#include <utf8.h>

#include <tulip/IntegerProperty.h>
#include <tulip/LabelsRenderer.h>
#include <tulip/GlUtils.h>
#include <tulip/NanoVGManager.h>
#include <tulip/GlyphsManager.h>
#include <tulip/TulipViewSettings.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/ParametricCurves.h>

using namespace std;
using namespace tlp;

static vector<string> splitStringToLines(const string &text) {
  vector<string> textVector;
  size_t lastPos=0;
  size_t pos=text.find_first_of("\n");

  while(pos!=string::npos) {
    textVector.push_back(text.substr(lastPos,pos-lastPos));
    lastPos=pos+1;
    pos=text.find_first_of("\n",pos+1);
  }
  textVector.push_back(text.substr(lastPos));
  return textVector;
}

Vec2f computeScreenPos(const MatrixGL &transformMatrix, const Vec4i &viewport, const Vec3f &point) {
  Vec4f screenCoord = Vec4f(point, 1.0) * transformMatrix;
  screenCoord /= screenCoord[3];
  screenCoord[0] = (screenCoord[0]+1.)*viewport[2]/2. + viewport[0];
  screenCoord[1] = (screenCoord[1]+1.)*viewport[3]/2. + viewport[1];
  return Vec2f(screenCoord[0], screenCoord[1]);
}

static Vec3f rotatePoint(const Vec3f &center, float angle, const Vec3f &p) {
  float s = sin(angle);
  float c = cos(angle);
  return Vec3f(center[0]+(p[0]-center[0])*c - (p[1]-center[1])*s,
               center[1]+(p[0]-center[0])*s + (p[1]-center[1])*c);
}

map<string, LabelsRenderer *> LabelsRenderer::_instances;
string LabelsRenderer::_currentCanvasId("");

LabelsRenderer* LabelsRenderer::instance() {
  return instance(_currentCanvasId);
}

LabelsRenderer *LabelsRenderer::instance(const string &canvasId) {
  if (_instances.find(canvasId) == _instances.end()) {
    _instances[canvasId] = new LabelsRenderer();
  }
  return _instances[canvasId];
}

LabelsRenderer::LabelsRenderer() :
  _labelsScaled(false), _minSize(12), _maxSize(72),
  _occlusionTest(true), _useFixedFontSize(false) {
}

void LabelsRenderer::initFont(const string &fontFile) {
  if (_fontHandles.find(fontFile) == _fontHandles.end()) {
    _fontHandles[fontFile] = -1;
    if (fileExists(fontFile.c_str())) {
      _fontHandles[fontFile] = nvgCreateFont(NanoVGManager::instance()->getNanoVGContext(), fontFile.c_str(), fontFile.c_str());
      if (_fontHandles[fontFile] == -1) {
        error() << "Font file " << fontFile << " can not be loaded" << endl;
      }
    } else {
      error() << fontFile << " is not a valid path, font can not be loaded" << endl;
    }
  }
}

void LabelsRenderer::setFont(const string &fontFile) {
  if (_fontHandles[fontFile] != -1) {
    nvgFontFaceId(NanoVGManager::instance()->getNanoVGContext(), _fontHandles[fontFile]);
    _currentFont = fontFile;
  } else {
    setFont(TulipViewSettings::instance().defaultFontFile());
  }
}

void LabelsRenderer::renderText(NVGcontext *vg, const string &text, const BoundingBox &renderingBox,
                                const Color &textColor, int fontSize, float rotation) {

  vector<string> textVector = splitStringToLines(text);

  float lineHeight = renderingBox.height()/textVector.size();

  if (_useFixedFontSize && !_labelsScaled) {
    nvgFontSize(vg, fontSize);
  } else {
    nvgFontSize(vg, lineHeight);
  }
  nvgTextAlign(vg, NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
  nvgFillColor(vg, nvgRGBA(textColor[0],textColor[1],textColor[2],textColor[3]));
  for (size_t i = 0 ; i < textVector.size() ; ++i) {
    BoundingBox rb;
    rb[0] = Vec3f(renderingBox[0][0], renderingBox[0][1] + i * lineHeight);
    rb[1] = Vec3f(renderingBox[1][0], renderingBox[0][1] + (i+1) * lineHeight);
    nvgTranslate(vg, rb.center()[0], rb.center()[1]);
    nvgRotate(vg, rotation);
    nvgText(vg, 0, 0, textVector[i].c_str(), nullptr);
    nvgResetTransform(vg);
  }

}

BoundingBox LabelsRenderer::getLabelRenderingBoxScaled(const BoundingBox &renderingBox, float textAspectRatio) {
  BoundingBox renderingBoxScaled;

  float ratio = renderingBox.width() / renderingBox.height();

  float scaleX = 0;
  float scaleY = 0;

  if (ratio > textAspectRatio) {
    scaleX = renderingBox.height() * textAspectRatio;
    scaleY = renderingBox.height();
  } else {
    scaleX = renderingBox.width();
    scaleY = renderingBox.width() / textAspectRatio;
  }

  float z = renderingBox[1][2];

  renderingBoxScaled[0] = Coord(renderingBox.center()[0] - scaleX / 2.f, renderingBox.center()[1] - scaleY / 2.f, z);
  renderingBoxScaled[1] = Coord(renderingBox.center()[0] + scaleX / 2.f, renderingBox.center()[1] + scaleY / 2.f, z);

  return renderingBoxScaled;
}

BoundingBox LabelsRenderer::getTextBoundingBox(const string &text, int fontSize) {

  vector<string> textVector = splitStringToLines(text);

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  if (_useFixedFontSize) {
    nvgFontSize(vg, fontSize);
  }

  float width = 0, height = 0;
  for (size_t i = 0 ; i < textVector.size() ; ++i) {
    float bounds[4];
    nvgTextBounds(vg, 0, 0, textVector[i].c_str(), nullptr, bounds);
    width = max(width, bounds[2] - bounds[0]);
    height += (bounds[3] - bounds[1]);
  }

  return BoundingBox(Coord(0,0), Coord(width, height));

}

void LabelsRenderer::renderOneLabel(const Camera &camera, const string &text, const BoundingBox &renderingBox,
                                    const Color &labelColor, int fontSize, const string &fontFile) {

  if (text.empty()) return;

  if (!fontFile.empty()) {
    initFont(fontFile);
    setFont(fontFile);
  } else {
    initFont(TulipViewSettings::instance().defaultFontFile());
    setFont(TulipViewSettings::instance().defaultFontFile());
  }

  BoundingBox textBB = getTextBoundingBox(text, fontSize);

  BoundingBox textBBScaled = getLabelRenderingBoxScaled(renderingBox, textBB.width()/textBB.height());

  Vec4i viewport = camera.getViewport();

  Vec2f textBBMinScr = computeScreenPos(camera.transformMatrix(), viewport, textBBScaled[0]);
  Vec2f textBBMaxScr = computeScreenPos(camera.transformMatrix(), viewport, textBBScaled[1]);

  BoundingBox bb;
  bb.expand(Vec3f(textBBMinScr[0], viewport[3] - textBBMinScr[1]));
  bb.expand(Vec3f(textBBMaxScr[0], viewport[3] - textBBMaxScr[1]));

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  nvgBeginFrame(vg, camera.getViewport()[0], camera.getViewport()[1], camera.getViewport()[2], camera.getViewport()[3], 1.0);

  renderText(vg, text, bb, labelColor, fontSize);

  nvgEndFrame(vg);
}

static BoundingBox labelBoundingBoxForNode(const GlGraphInputData &inputData, node n) {
  BoundingBox renderingBox;
  GlyphsManager::instance()->getGlyph(inputData.getElementShape()->getNodeValue(n))->getTextBoundingBox(renderingBox);
  const Coord &nodePos = inputData.getElementLayout()->getNodeValue(n);
  const Size &nodeSize = inputData.getElementSize()->getNodeValue(n);
  Size labelSize = nodeSize* Size(renderingBox.width(), renderingBox.height(), renderingBox.depth());
  Coord pos = nodePos;
  int labelPos = inputData.getElementLabelPosition()->getNodeValue(n);
  if (labelPos == LabelPosition::Top) {
    pos += Coord(0, nodeSize[1]/2.f+labelSize[1]/2.f);
  } else if (labelPos == LabelPosition::Bottom) {
    pos += Coord(0, -nodeSize[1]/2.f-labelSize[1]/2.f);
  } else if (labelPos == LabelPosition::Left) {
    pos += Coord(-nodeSize[0]/2.f-labelSize[0]/2.f, 0);
  } else if (labelPos == LabelPosition::Right) {
    pos += Coord(nodeSize[0]/2.f+labelSize[0]/2.f, 0);
  }
  return BoundingBox(pos - labelSize/2.f, pos + labelSize/2.f);
}

static Coord getPointOnPolylineAt(const vector<Coord> &polyline, float t) {

  if (t == 0) {
    return polyline.front();
  } else if (t == 1.0) {
    return polyline.back();
  }

  vector<float> ts;
  ts.resize(polyline.size());
  float totalDist = 0;
  for (size_t i = 0 ; i < polyline.size() - 1 ; ++i) {
    totalDist += polyline[i].dist(polyline[i+1]);
  }
  float dist = 0;
  for (size_t i = 0 ; i < polyline.size() - 1 ; ++i) {
    ts[i] = dist / totalDist;
    dist += polyline[i].dist(polyline[i+1]);
  }
  ts[polyline.size() - 1] = 1.0;

  size_t idx = 0;
  while (t >= ts[idx+1]) {
    ++idx;
  }

  Coord point = polyline[idx] + (t-ts[idx])/(ts[idx+1] - ts[idx]) * (polyline[idx+1] - polyline[idx]);

  return point;

}

static pair<BoundingBox, float> labelBoundingBoxAndAngleForEdge(const Camera &camera, const GlGraphInputData &inputData, edge e) {
  Coord srcPos = inputData.getElementLayout()->getNodeValue(inputData.getGraph()->source(e));
  Coord tgtPos = inputData.getElementLayout()->getNodeValue(inputData.getGraph()->target(e));
  const Size &srcSize = inputData.getElementSize()->getNodeValue(inputData.getGraph()->source(e));
  const Size &tgtSize = inputData.getElementSize()->getNodeValue(inputData.getGraph()->target(e));
  double srcRot = inputData.getElementRotation()->getNodeValue(inputData.getGraph()->source(e));
  double tgtRot = inputData.getElementRotation()->getNodeValue(inputData.getGraph()->target(e));
  int srcGlyphId = inputData.getElementShape()->getNodeValue(inputData.getGraph()->source(e));
  int tgtGlyphId = inputData.getElementShape()->getNodeValue(inputData.getGraph()->target(e));
  const vector<Coord> &bends = inputData.getElementLayout()->getEdgeValue(e);
  const Size &edgeSize = inputData.getElementSize()->getEdgeValue(e);
  int edgeShape = inputData.getElementShape()->getEdgeValue(e);

  Coord srcAnchor = (bends.size() > 0) ? bends.front() : tgtPos;
  srcAnchor = GlyphsManager::instance()->getGlyph(srcGlyphId)->getAnchor(srcPos, srcAnchor, srcSize, srcRot);

  Coord tgtAnchor = (bends.size() > 0) ? bends.back() : srcPos;
  tgtAnchor = GlyphsManager::instance()->getGlyph(tgtGlyphId)->getAnchor(tgtPos, tgtAnchor, tgtSize, tgtRot);

  float dist = srcAnchor.dist(tgtAnchor);

  Coord center;
  float angle = 0;

  if (bends.empty()) {
    center = (srcPos + tgtPos) / 2.f;
    srcPos = Vec4f(srcPos, 1) * camera.modelviewMatrix();
    tgtPos = Vec4f(tgtPos, 1) * camera.modelviewMatrix();
    angle = atan((tgtPos[1]-srcPos[1])/(tgtPos[0]-srcPos[0]));
  } else {
    vector<Coord> controlPoints = {srcPos};
    controlPoints.insert(controlPoints.end(), bends.begin(), bends.end());
    controlPoints.push_back(tgtPos);
    Coord prevPoint;
    if (edgeShape == EdgeShape::BezierCurve ||
        (edgeShape == EdgeShape::CubicBSplineCurve && controlPoints.size() <= 3)) {
      prevPoint = computeBezierPoint(controlPoints, 0.45);
      center = computeBezierPoint(controlPoints, 0.5);
    } else if (edgeShape == EdgeShape::Polyline) {
      prevPoint = getPointOnPolylineAt(controlPoints, 0.45);
      center = getPointOnPolylineAt(controlPoints, 0.5);
    } else if (edgeShape == EdgeShape::CubicBSplineCurve) {
      prevPoint = computeOpenUniformBsplinePoint(controlPoints, 0.45);
      center = computeOpenUniformBsplinePoint(controlPoints, 0.5);
    } else {
      prevPoint = computeCatmullRomPoint(controlPoints, 0.45);
      center = computeCatmullRomPoint(controlPoints, 0.5);
    }
    Coord centerProj = Vec4f(center, 1) * camera.modelviewMatrix();
    Coord prevPointProj = Vec4f(prevPoint, 1) * camera.modelviewMatrix();
    angle=atan((centerProj[1]-prevPointProj[1])/(centerProj[0]-prevPointProj[0]));
  }

  BoundingBox renderingBox;
  renderingBox.expand(center - Vec3f(dist/3.f, 4*edgeSize[1]));
  renderingBox.expand(center + Vec3f(dist/3.f, 4*edgeSize[1]));

  return make_pair(renderingBox, -angle);
}

static void adjustTextBoundingBox(BoundingBox &textBB, const Camera &camera, float minSize, float maxSize, unsigned int nbLines) {
  Vec4i viewport = camera.getViewport();
  Vec2f textBBMinScr = computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[0]);
  Vec2f textBBMaxScr = computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[1]);

  float screenH = (textBBMaxScr[1] - textBBMinScr[1]) / nbLines;

  Vec3f center = textBB.center();
  Vec3f scale = textBB[1] - textBB[0];
  float ratio = textBB.width() / textBB.height();
  float scaleX = textBB.width();

  if (screenH < minSize) {
    scaleX *= (minSize/screenH);
  }
  if (screenH > maxSize) {
    scaleX *= (maxSize/screenH);
  }

  float scaleY = scaleX/ratio;

  textBB[0][0] = ((textBB[0][0] - center[0]) / scale[0]) * scaleX + center[0];
  textBB[0][1] = ((textBB[0][1] - center[1]) / scale[1]) * scaleY + center[1];
  textBB[1][0] = ((textBB[1][0] - center[0]) / scale[0]) * scaleX + center[0];
  textBB[1][1] = ((textBB[1][1] - center[1]) / scale[1]) * scaleY + center[1];
}

void LabelsRenderer::renderGraphElementsLabels(const GlGraphInputData &inputData, const Camera &camera, const Color &selectionColor, int labelsDensity) {

  if (labelsDensity == -100) return;

  initFont(TulipViewSettings::instance().defaultFontFile());

  Vec4i viewport = camera.getViewport();

  vector<vector<Vec2f> > renderedLabelsScrRect;
  vector<BoundingBox> renderedLabelsScrBB;

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  nvgBeginFrame(vg, camera.getViewport()[0], camera.getViewport()[1], camera.getViewport()[2], camera.getViewport()[3], 1.0);

  Graph *graph = inputData.getGraph();

  for (node n :_nodesLabelsToRender[graph]) {

    setFont(inputData.getElementFont()->getNodeValue(n));

    const string &label = inputData.getElementLabel()->getNodeValue(n);

    if (label.empty()) {
      continue;
    }

    BoundingBox textBBFont = getTextBoundingBox(label, inputData.getElementFontSize()->getNodeValue(n));

    float nodeLabelAspectRatio = textBBFont.width() / textBBFont.height();
    float nodeLabelNbLines = count(label.begin(), label.end(), '\n') + 1;

    BoundingBox nodeBB = labelBoundingBoxForNode(inputData, n);

    BoundingBox textBB = getLabelRenderingBoxScaled(nodeBB, nodeLabelAspectRatio);

    if (!_labelsScaled && !_useFixedFontSize) {
      adjustTextBoundingBox(textBB, camera, _minSize, _maxSize, nodeLabelNbLines);
    }

    bool canRender = true;

    BoundingBox textScrBB;
    BoundingBox textScrBBModified;
    textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[0]), 0));
    textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[1]), 0));
    float width = textScrBB.width();
    float height = textScrBB.height();
    Coord centerScr = Vec3f(computeScreenPos(camera.transformMatrix(), viewport, textBB.center()), 0);
    centerScr[1] = viewport[3] - centerScr[1];

    if (!_useFixedFontSize || _labelsScaled) {
      textScrBB[0] = centerScr - Vec3f(width/2, height/2);
      textScrBB[1] = centerScr + Vec3f(width/2, height/2);
    } else {
      textScrBB[0] = centerScr - Vec3f(textBBFont.width()/2, textBBFont.height()/2);
      textScrBB[1] = centerScr + Vec3f(textBBFont.width()/2, textBBFont.height()/2);
    }

    if (_occlusionTest && labelsDensity != 100) {

      float newWidth, newHeight;
      if (labelsDensity < 0) {
        newWidth = textScrBB.width() + (-labelsDensity/100.f) * textScrBB.width();
        newHeight = textScrBB.height() + (-labelsDensity/100.f) * textScrBB.height();
      } else {
        newWidth = textScrBB.width() - (labelsDensity/100.f) * textScrBB.width();
        newHeight = textScrBB.height() - (labelsDensity/100.f) * textScrBB.height();
      }

      textScrBBModified[0] = textScrBB.center() - Coord(newWidth/2, newHeight/2);
      textScrBBModified[1] = textScrBB.center() + Coord(newWidth/2, newHeight/2);

      for (size_t i = 0 ; i < renderedLabelsScrBB.size() ; ++i) {
        if (textScrBBModified.intersect(renderedLabelsScrBB[i])) {
          canRender = false;
          break;
        }
      }

    }

    if (canRender) {
      renderText(vg, label, textScrBB, inputData.getElementSelection()->getNodeValue(n) ? selectionColor : inputData.getElementLabelColor()->getNodeValue(n),
                 inputData.getElementFontSize()->getNodeValue(n));
      renderedLabelsScrBB.push_back(textScrBBModified);
    }

  }

  for (edge e : _edgesLabelsToRender[graph]) {

    bool canRender = true;

    setFont(inputData.getElementFont()->getEdgeValue(e));

    const string &label = inputData.getElementLabel()->getEdgeValue(e);

    if (label.empty()) {
      continue;
    }

    BoundingBox textBBFont = getTextBoundingBox(label, inputData.getElementFontSize()->getEdgeValue(e));

    float edgeLabelAspectRatio = textBBFont.width() / textBBFont.height();
    float edgeLabelNbLines = count(label.begin(), label.end(), '\n') + 1;

    auto edgeBB = labelBoundingBoxAndAngleForEdge(camera, inputData, e);

    BoundingBox textBB = getLabelRenderingBoxScaled(edgeBB.first, edgeLabelAspectRatio);

    if (!_labelsScaled && !_useFixedFontSize) {
      adjustTextBoundingBox(textBB, camera, _minSize, _maxSize, edgeLabelNbLines);
    }

    BoundingBox textScrBB;
    BoundingBox textScrBBModified;
    textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[0]), 0));
    textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[1]), 0));
    float width = textScrBB.width();
    float height = textScrBB.height();
    Coord centerScr = Vec3f(computeScreenPos(camera.transformMatrix(), viewport, textBB.center()), 0);
    centerScr[1] = viewport[3] - centerScr[1];

    if (!_useFixedFontSize || _labelsScaled) {
      textScrBB[0] = centerScr - Vec3f(width/2, height/2);
      textScrBB[1] = centerScr + Vec3f(width/2, height/2);
    } else {
      textScrBB[0] = centerScr - Vec3f(textBBFont.width()/2, textBBFont.height()/2);
      textScrBB[1] = centerScr + Vec3f(textBBFont.width()/2, textBBFont.height()/2);
    }

    if (_occlusionTest && labelsDensity != 100) {

      float newWidth, newHeight;
      if (labelsDensity < 0) {
        newWidth = textScrBB.width() + (-labelsDensity/100.f) * textScrBB.width();
        newHeight = textScrBB.height() + (-labelsDensity/100.f) * textScrBB.height();
      } else {
        newWidth = textScrBB.width() - (labelsDensity/100.f) * textScrBB.width();
        newHeight = textScrBB.height() - (labelsDensity/100.f) * textScrBB.height();
      }

      textScrBBModified[0] = textScrBB.center() - Coord(newWidth/2, newHeight/2);
      textScrBBModified[1] = textScrBB.center() + Coord(newWidth/2, newHeight/2);

    } else {
      textScrBBModified = textScrBB;
    }

    vector<Vec2f> textScrRect;
    textScrRect.push_back(rotatePoint(textScrBBModified.center(), edgeBB.second, textScrBBModified[0]));
    textScrRect.push_back(rotatePoint(textScrBBModified.center(), edgeBB.second, Vec3f(textScrBBModified[0][0]+textScrBBModified.width(), textScrBBModified[0][1], textScrBBModified[0][2])));
    textScrRect.push_back(rotatePoint(textScrBBModified.center(), edgeBB.second, textScrBBModified[1]));
    textScrRect.push_back(rotatePoint(textScrBBModified.center(), edgeBB.second, Vec3f(textScrBBModified[0][0], textScrBBModified[0][1]+textScrBBModified.height(), textScrBBModified[0][2])));

    if (_occlusionTest && labelsDensity != 100) {

      for (size_t i = 0 ; i < renderedLabelsScrRect.size() ; ++i) {
        if (convexPolygonsIntersect(textScrRect, renderedLabelsScrRect[i])) {
          canRender = false;
          break;
        }
      }

      if (canRender) {

        for (size_t i = 0 ; i < renderedLabelsScrBB.size() ; ++i) {
          vector<Vec2f> textScrBB;
          textScrBB.push_back(Vec2f(renderedLabelsScrBB[i][0]));
          textScrBB.push_back(Vec2f(renderedLabelsScrBB[i][0])+Vec2f(renderedLabelsScrBB[i].width(), 0));
          textScrBB.push_back(Vec2f(renderedLabelsScrBB[i][1]));
          textScrBB.push_back(Vec2f(renderedLabelsScrBB[i][0])+Vec2f(0,renderedLabelsScrBB[i].height()));

          if (convexPolygonsIntersect(textScrRect, textScrBB)) {
            canRender = false;
            break;
          }
        }
      }

      if (canRender) {
        renderedLabelsScrRect.push_back(textScrRect);
        renderText(vg, label, textScrBB, inputData.getElementSelection()->getEdgeValue(e) ? selectionColor : inputData.getElementLabelColor()->getEdgeValue(e),
                   inputData.getElementFontSize()->getEdgeValue(e), edgeBB.second);
      }

    }
  }

  nvgEndFrame(vg);

}
