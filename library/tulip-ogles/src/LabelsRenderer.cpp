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

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <tulip/IntegerProperty.h>

#include <tulip/LabelsRenderer.h>
#include <tulip/GlUtils.h>
#include <utf8.h>
#include <tulip/NanoVGManager.h>
#include <tulip/glyphs/GlyphsManager.h>
#include <tulip/TulipViewSettings.h>

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


#ifdef __EMSCRIPTEN__

static set<string> fontFileRequested;

static void fontFileLoaded(const char *fontFile) {
  cout << "Font file " << fontFile << " successfully loaded" << endl;
}

static void fontFileLoadError(const char *fontFile) {
  cout << "Error when trying to load font file " << fontFile << endl;
}
#endif

static void renderText(NVGcontext *vg, const std::string &text, const tlp::BoundingBox &renderingBox, const tlp::Color &textColor)  {

  vector<string> textVector = splitStringToLines(text);

  float fontSize = renderingBox.height()/textVector.size();

  nvgFontSize(vg, fontSize);
  nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
  nvgFillColor(vg, nvgRGBA(textColor[0],textColor[1],textColor[2],textColor[3]));
  for (size_t i = 0 ; i < textVector.size() ; ++i) {
    BoundingBox rb;
    rb[0] = Vec3f(renderingBox[0][0], renderingBox[0][1] + i * fontSize);
    rb[1] = Vec3f(renderingBox[1][0], renderingBox[0][1] + (i+1) * fontSize);
    nvgText(vg, rb.center()[0], rb.center()[1], textVector[i].c_str(), nullptr);
  }

}

map<string, LabelsRenderer *> LabelsRenderer::_instances;
string LabelsRenderer::_currentCanvasId("");

LabelsRenderer* LabelsRenderer::instance() {
  return instance(_currentCanvasId);
}

LabelsRenderer *LabelsRenderer::instance(const std::string &canvasId) {
  if (_instances.find(canvasId) == _instances.end()) {
    _instances[canvasId] = new LabelsRenderer();
  }
  return _instances[canvasId];
}

LabelsRenderer::LabelsRenderer() :
  _fontHandle(-1), _labelsScaled(false),
  _minSize(12), _maxSize(72), _occlusionTest(true) {

  setFontFile(TulipViewSettings::instance().defaultFontFile());

}

LabelsRenderer::~LabelsRenderer() {

}

void LabelsRenderer::initFont() {
  if (_fontFile.empty()) return;
  if (!fontInit()) {
  #ifdef __EMSCRIPTEN__
    if (!fileExists(_fontFile.c_str()) && fontFileRequested.find(_fontFile) == fontFileRequested.end()) {
      fontFileRequested.insert(_fontFile);
      emscripten_async_wget(_fontFile.c_str(), _fontFile.c_str(), fontFileLoaded, fontFileLoadError);
      return;
    }
  #endif
    if (fileExists(_fontFile.c_str())) {
      _fontHandle = nvgCreateFont(NanoVGManager::instance()->getNanoVGContext(), _fontFile.c_str(), _fontFile.c_str());
    } else {
      std::cout << _fontFile << " is not loaded" << std::endl;
    }
  } else {
    nvgFontFace(NanoVGManager::instance()->getNanoVGContext(), _fontFile.c_str());
  }
}

void LabelsRenderer::addOrUpdateNodeLabel(Graph *graph, node n) {
  StringProperty *viewLabel = graph->getProperty<StringProperty>("viewLabel");
  const string &label = viewLabel->getNodeValue(n);
  if (!label.empty()) {
    _nodeLabelAspectRatio[graph][n] = getTextAspectRatio(label);
    _nodeLabelNbLines[graph][n] = std::count(label.begin(), label.end(), '\n')+1;
  }
}

void LabelsRenderer::removeNodeLabel(tlp::Graph *graph, tlp::node n) {
  _nodeLabelAspectRatio[graph].erase(n);
  _nodeLabelNbLines[graph].erase(n);
}

Vec2f computeScreenPos(const MatrixGL &transformMatrix, const Vec4i &viewport, const Vec3f &point) {
  Vec4f screenCoord = Vec4f(point, 1.0) * transformMatrix;
  screenCoord /= screenCoord[3];
  screenCoord[0] = (screenCoord[0]+1.)*viewport[2]/2. + viewport[0];
  screenCoord[1] = (screenCoord[1]+1.)*viewport[3]/2. + viewport[1];
  return Vec2f(screenCoord[0], screenCoord[1]);
}

tlp::BoundingBox LabelsRenderer::getLabelRenderingBoxScaled(const tlp::BoundingBox &renderingBox, float textAspectRatio) {
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

float LabelsRenderer::getTextAspectRatio(const string &text) {

  vector<string> textVector = splitStringToLines(text);

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  float width = 0, height = 0;
  for (size_t i = 0 ; i < textVector.size() ; ++i) {
    float bounds[4];
    nvgTextBounds(vg, 0, 0, textVector[i].c_str(), nullptr, bounds);
    width = max(width, bounds[2] - bounds[0]);
    height += (bounds[3] - bounds[1]);
  }

  return width/height;

}

void LabelsRenderer::renderOneLabel(const Camera &camera, const string &text, const BoundingBox &renderingBox, const Color &labelColor) {
  initFont();
  if (text.empty()) return;

  BoundingBox textBBScaled = getLabelRenderingBoxScaled(renderingBox, getTextAspectRatio(text));

  Vec4i viewport = camera.getViewport();

  Vec2f textBBMinScr = computeScreenPos(camera.transformMatrix(), viewport, textBBScaled[0]);
  Vec2f textBBMaxScr = computeScreenPos(camera.transformMatrix(), viewport, textBBScaled[1]);

  BoundingBox bb;
  bb.expand(Vec3f(textBBMinScr[0], viewport[3] - textBBMinScr[1]));
  bb.expand(Vec3f(textBBMaxScr[0], viewport[3] - textBBMaxScr[1]));

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  nvgBeginFrame(vg, camera.getViewport()[0], camera.getViewport()[1], camera.getViewport()[2], camera.getViewport()[3], 1.0);

  renderText(vg, text, bb, labelColor);

  nvgEndFrame(vg);
}

static BoundingBox labelBoundingBoxForNode(Graph *graph, node n) {
  IntegerProperty *viewShape = graph->getProperty<IntegerProperty>("viewShape");
  LayoutProperty *viewLayout = graph->getProperty<LayoutProperty>("viewLayout");
  SizeProperty *viewSize = graph->getProperty<SizeProperty>("viewSize");
  BoundingBox renderingBox;
  GlyphsManager::getGlyph(viewShape->getNodeValue(n))->getTextBoundingBox(renderingBox);
  const Coord &pos = viewLayout->getNodeValue(n);
  const Size &size = viewSize->getNodeValue(n) * Size(renderingBox.width(), renderingBox.height(), renderingBox.depth());
  return BoundingBox(pos - size/2.f, pos + size/2.f);
}

static void adjustTextBoundingBox(BoundingBox &textBB, const Camera &camera, float minSize, float maxSize, unsigned int nbLines) {
  Vec4i viewport = camera.getViewport();
  Vec2f textBBMinScr = computeScreenPos(camera.transformMatrix(), viewport, textBB[0]);
  Vec2f textBBMaxScr = computeScreenPos(camera.transformMatrix(), viewport, textBB[1]);

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

void LabelsRenderer::renderGraphNodesLabels(Graph *graph, const Camera &camera, const Color &selectionColor) {

  initFont();

  BooleanProperty *viewSelection = graph->getProperty<BooleanProperty>("viewSelection");
  ColorProperty *viewLabelColor = graph->getProperty<ColorProperty>("viewLabelColor");
  StringProperty *viewLabel = graph->getProperty<StringProperty>("viewLabel");

  Vec4i viewport = camera.getViewport();

  vector<vector<Vec2f> > renderedLabelsScrRect;
  vector<BoundingBox> renderedLabelsScrBB;

  NVGcontext* vg = NanoVGManager::instance()->getNanoVGContext();

  nvgBeginFrame(vg, camera.getViewport()[0], camera.getViewport()[1], camera.getViewport()[2], camera.getViewport()[3], 1.0);

  for (vector<node>::iterator it = _labelsToRender[graph].begin() ; it != _labelsToRender[graph].end() ; ++it) {

    const std::string &label = viewLabel->getNodeValue(*it);

    if (label.empty()) {
      continue;
    }

    if (_nodeLabelAspectRatio[graph].find(*it) == _nodeLabelAspectRatio[graph].end()) {
      _nodeLabelAspectRatio[graph][*it] = getTextAspectRatio(label);
      _nodeLabelNbLines[graph][*it] = std::count(label.begin(), label.end(), '\n')+1;
    }

    BoundingBox nodeBB = labelBoundingBoxForNode(graph, *it);

    BoundingBox textBB = getLabelRenderingBoxScaled(nodeBB, _nodeLabelAspectRatio[graph][*it]);

    if (!_labelsScaled) {

      adjustTextBoundingBox(textBB, camera, _minSize, _maxSize, _nodeLabelNbLines[graph][*it]);
    }

    bool canRender = true;

    if (_occlusionTest) {

      //if (!camera.hasRotation()) {
        BoundingBox textScrBB;
        textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[0]), 0));
        textScrBB.expand(Vec3f(computeScreenPos(camera.transformMatrixBillboard(), viewport, textBB[1]), 0));

        for (size_t i = 0 ; i < renderedLabelsScrBB.size() ; ++i) {
          if (textScrBB.intersect(renderedLabelsScrBB[i])) {
            canRender = false;
            break;
          }
        }

        if (canRender) {
          renderedLabelsScrBB.push_back(textScrBB);
        }

//      } else {
//        vector<Vec2f> textScrRect;
//        textScrRect.push_back(computeScreenPos(camera.transformMatrix(), viewport, textBB[0]));
//        textScrRect.push_back(computeScreenPos(camera.transformMatrix(), viewport, Vec3f(textBB[0][0]+textBB.width(), textBB[0][1], textBB[0][2])));
//        textScrRect.push_back(computeScreenPos(camera.transformMatrix(), viewport, textBB[1]));
//        textScrRect.push_back(computeScreenPos(camera.transformMatrix(), viewport, Vec3f(textBB[0][0], textBB[0][1]+textBB.height(), textBB[0][2])));

//        for (size_t i = 0 ; i < renderedLabelsScrRect.size() ; ++i) {
//          if (convexPolygonsIntersect(textScrRect, renderedLabelsScrRect[i])) {
//            canRender = false;
//            break;
//          }
//        }

//        if (canRender) {
//          renderedLabelsScrRect.push_back(textScrRect);
//        }
//      }
    }

    if (canRender) {

      Vec2f textBBMinScr = computeScreenPos(camera.transformMatrix(), viewport, textBB[0]);
      Vec2f textBBMaxScr = computeScreenPos(camera.transformMatrix(), viewport, textBB[1]);
      BoundingBox bb;
      bb.expand(Vec3f(textBBMinScr[0], viewport[3] - textBBMinScr[1]));
      bb.expand(Vec3f(textBBMaxScr[0], viewport[3] - textBBMaxScr[1]));

      renderText(vg, viewLabel->getNodeValue(*it), bb, viewSelection->getNodeValue(*it) ? selectionColor : viewLabelColor->getNodeValue(*it));

    }

  }

  nvgEndFrame(vg);
}

void LabelsRenderer::clearGraphNodesLabelsRenderingData(Graph *graph) {
  if (!graph) return;
  _nodeLabelNbLines.erase(graph);
  _nodeLabelAspectRatio.erase(graph);
}


