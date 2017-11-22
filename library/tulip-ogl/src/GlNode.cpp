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

#include <climits>
#include <set>

#include <tulip/GlNode.h>

#include <tulip/Coord.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlMetaNodeRenderer.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/Glyph.h>
#include <tulip/GlTools.h>
#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlLabel.h>
#include <tulip/GlBox.h>
#include <tulip/GlGlyphRenderer.h>
#include <tulip/TulipViewSettings.h>

//====================================================
tlp::GlLabel *tlp::GlNode::label = nullptr;
tlp::GlBox *tlp::GlNode::selectionBox = nullptr;

#define LOD_MIN_TRESHOLD 10.0

using namespace std;

namespace tlp {

GlNode::GlNode(unsigned int id) : id(id), oldId(UINT_MAX) {
  if (!label)
    label = new GlLabel();

  if (!selectionBox) {
    selectionBox = new GlBox(Coord(0, 0, 0), Size(1, 1, 1), Color(0, 0, 255, 255),
                             Color(0, 255, 0, 255), false, true);
    selectionBox->setOutlineSize(3);
  }
}

void GlNode::init(const GlGraphInputData *data) {
  if (id != oldId) {
    node n(id);
    oldId = id;
    coord = data->getElementLayout()->getNodeValue(n);
    glyph = data->getElementShape()->getNodeValue(n);
    size = data->getElementSize()->getNodeValue(n);
    rot = data->getElementRotation()->getNodeValue(n);
    selected = data->getElementSelected()->getNodeValue(n);
  }
}

BoundingBox GlNode::getBoundingBox(const GlGraphInputData *data) {
  init(data);

  if (rot == 0) {
    BoundingBox box;
    box.expand(coord - size / 2.f);
    box.expand(coord + size / 2.f);
    assert(box.isValid());
    return box;
  } else {
    float cosAngle = cos(rot / 180. * M_PI);
    float sinAngle = sin(rot / 180. * M_PI);
    Coord tmp1(size / 2.f);
    Coord tmp2(tmp1[0], -tmp1[1], tmp1[2]);
    Coord tmp3(-tmp1[0], -tmp1[1], -tmp1[2]);
    Coord tmp4(-tmp1[0], tmp1[1], -tmp1[2]);
    tmp1 = Coord(tmp1[0] * cosAngle - tmp1[1] * sinAngle, tmp1[0] * sinAngle + tmp1[1] * cosAngle,
                 tmp1[2]);
    tmp2 = Coord(tmp2[0] * cosAngle - tmp2[1] * sinAngle, tmp2[0] * sinAngle + tmp2[1] * cosAngle,
                 tmp2[2]);
    tmp3 = Coord(tmp3[0] * cosAngle - tmp3[1] * sinAngle, tmp3[0] * sinAngle + tmp3[1] * cosAngle,
                 tmp3[2]);
    tmp4 = Coord(tmp4[0] * cosAngle - tmp4[1] * sinAngle, tmp4[0] * sinAngle + tmp4[1] * cosAngle,
                 tmp4[2]);
    BoundingBox bb;
    bb.expand(coord + tmp1);
    bb.expand(coord + tmp2);
    bb.expand(coord + tmp3);
    bb.expand(coord + tmp4);
    return bb;
  }
}

void GlNode::acceptVisitor(GlSceneVisitor *visitor) {
  visitor->visit(this);
}

void GlNode::draw(float lod, const GlGraphInputData *data, Camera *camera) {
  init(data);
  const Color &colorSelect2 = data->parameters->getSelectionColor();

  glEnable(GL_CULL_FACE);

  node n(id);

  // do not render metanode is lod is too low
  if (data->getGraph()->isMetaNode(n) && lod >= LOD_MIN_TRESHOLD) {
    data->getMetaNodeRenderer()->render(n, lod, camera);
  }

  const Color &fillColor = data->getElementColor()->getNodeValue(n);

  const Color &strokeColor = data->getElementBorderColor()->getNodeValue(n);

  const Color &textColor = data->getElementLabelColor()->getNodeValue(n);

  GlTextureManager::getInst().setAnimationFrame(data->getElementAnimationFrame()->getNodeValue(n));

  if (data->parameters->getFeedbackRender()) {
    glPassThrough(TLP_FB_COLOR_INFO);
    glPassThrough(fillColor[0]);
    glPassThrough(fillColor[1]);
    glPassThrough(fillColor[2]);
    glPassThrough(fillColor[3]);
    glPassThrough(strokeColor[0]);
    glPassThrough(strokeColor[1]);
    glPassThrough(strokeColor[2]);
    glPassThrough(strokeColor[3]);
    glPassThrough(textColor[0]);
    glPassThrough(textColor[1]);
    glPassThrough(textColor[2]);
    glPassThrough(textColor[3]);

    glPassThrough(TLP_FB_BEGIN_NODE);
    glPassThrough(id); // id of the node for the feed back mode
  }

  if (lod < LOD_MIN_TRESHOLD) { // less than four pixel on screen, we use points instead of glyphs
    if (lod < 1)
      lod = 1;

    if (data->getGlVertexArrayManager()->renderingIsBegin()) {
      data->getGlVertexArrayManager()->activatePointNodeDisplay(this, selected);
    } else {
      glDisable(GL_LIGHTING);
      setColor(selected ? colorSelect2 : ((data->getElementBorderWidth()->getNodeValue(n) > 0)
                                              ? data->getElementBorderColor()->getNodeValue(n)
                                              : data->getElementColor()->getNodeValue(n)));
      glPointSize(4);
      glBegin(GL_POINTS);
      glVertex3f(coord[0], coord[1], coord[2] + size[2] / 2.);
      glEnd();
      glEnable(GL_LIGHTING);
    }

    return;
  }

  if (!data->parameters->isDisplayNodes())
    return;

  Size nodeSize(size);

  // If node size in z is equal to 0 we have to scale with FLT_EPSILON to preserve normal
  // (because if we do a scale of (x,y,0) and if we have a normal like (0,0,1) the new normal after
  // scale will be (0,0,0) and we will have light problem)
  if (nodeSize[2] == 0)
    nodeSize[2] = FLT_EPSILON;

  // Some glyphs can not benefit from the shader rendering optimization
  // due to the use of quadrics or modelview matrix modification or lighting effect
  static set<int> noShaderGlyphs;

  if (noShaderGlyphs.empty()) {
    noShaderGlyphs.insert(NodeShape::Billboard);
    noShaderGlyphs.insert(NodeShape::ChristmasTree);
    noShaderGlyphs.insert(NodeShape::Cone);
    noShaderGlyphs.insert(NodeShape::Cylinder);
    noShaderGlyphs.insert(NodeShape::GlowSphere);
    noShaderGlyphs.insert(NodeShape::HalfCylinder);
    noShaderGlyphs.insert(NodeShape::Sphere);
    noShaderGlyphs.insert(NodeShape::Cube);
    noShaderGlyphs.insert(NodeShape::CubeOutlined);
  }

  if (data->getGlGlyphRenderer()->renderingHasStarted() &&
      noShaderGlyphs.find(glyph) == noShaderGlyphs.end()) {
    data->getGlGlyphRenderer()->addNodeGlyphRendering(data->glyphs.get(glyph), n, lod, coord,
                                                      nodeSize, rot, selected);
  } else {

    if (selected) {
      glStencilFunc(GL_LEQUAL, data->parameters->getSelectedNodesStencil(), 0xFFFF);
    } else {
      glStencilFunc(GL_LEQUAL, data->parameters->getNodesStencil(), 0xFFFF);
    }

    // draw a glyph or make recursive call for meta nodes
    glPushMatrix();
    glTranslatef(coord[0], coord[1], coord[2]);
    glRotatef(rot, 0., 0., 1.);

    glScalef(nodeSize[0], nodeSize[1], nodeSize[2]);

    if (selected) {
      selectionBox->setStencil(data->parameters->getSelectedNodesStencil() - 1);
      selectionBox->setOutlineColor(colorSelect2);
      selectionBox->draw(10, nullptr);
    }

    data->glyphs.get(glyph)->draw(n, lod);

    glPopMatrix();
  }

  GlTextureManager::getInst().setAnimationFrame(0);

  if (data->parameters->getFeedbackRender()) {
    glPassThrough(TLP_FB_END_NODE);
  }
}

void GlNode::drawLabel(bool drawSelect, OcclusionTest *test, const GlGraphInputData *data,
                       float lod) {
  init(data);

  if (drawSelect != selected)
    return;

  drawLabel(test, data, lod);
}

void GlNode::drawLabel(OcclusionTest *test, const GlGraphInputData *data) {
  GlNode::drawLabel(test, data, 1000.);
}

void GlNode::drawLabel(OcclusionTest *test, const GlGraphInputData *data, float lod,
                       Camera *camera) {
  init(data);
  node n(id);

  // If glyph cannot render label: return
  if (data->glyphs.get(glyph)->renderLabel())
    return;

  // Color of the label : selected or not
  const Color &fontColor = selected ? data->parameters->getSelectionColor()
                                    : data->getElementLabelColor()->getNodeValue(n);
  const Color &fontBorderColor = selected ? data->parameters->getSelectionColor()
                                          : data->getElementLabelBorderColor()->getNodeValue(n);
  float fontBorderWidth = data->getElementLabelBorderWidth()->getNodeValue(n);

  // If we have transparent label : return
  if (fontColor.getA() == 0 && (fontBorderColor.getA() == 0 || fontBorderWidth == 0))
    return;

  // Node text
  const string &tmp = data->getElementLabel()->getNodeValue(n);

  if (tmp.length() < 1)
    return;

  if (selected)
    label->setStencil(data->parameters->getSelectedNodesStencil());
  else
    label->setStencil(data->parameters->getNodesLabelStencil());

  int fontSize = data->getElementFontSize()->getNodeValue(n);

  if (fontSize <= 0)
    return;

  if (selected)
    fontSize += 2;

  int labelPos = data->getElementLabelPosition()->getNodeValue(n);

  BoundingBox includeBB;
  data->glyphs.get(glyph)->getTextBoundingBox(includeBB, n);
  Coord centerBB(includeBB.center());
  Vec3f sizeBB = includeBB[1] - includeBB[0];

  label->setFontNameSizeAndColor(data->getElementFont()->getNodeValue(n), fontSize, fontColor);
  label->setOutlineColor(fontBorderColor);
  label->setOutlineSize(fontBorderWidth);
  label->setText(tmp);
  label->setTranslationAfterRotation(centerBB * size);
  label->setSize(Size(size[0] * sizeBB[0], size[1] * sizeBB[1], 0));
  label->setSizeForOutAlign(Size(size[0], size[1], 0));
  label->rotate(0, 0, rot);
  label->setAlignment(labelPos);
  label->setScaleToSize(data->parameters->isLabelScaled());
  label->setUseLODOptimisation(true, this->getBoundingBox(data));
  label->setLabelsDensity(data->parameters->getLabelsDensity());
  label->setUseMinMaxSize(!data->parameters->isLabelFixedFontSize());
  label->setMinSize(data->parameters->getMinSizeOfLabel());
  label->setMaxSize(data->parameters->getMaxSizeOfLabel());
  label->setOcclusionTester(test);
  label->setBillboarded(data->parameters->getLabelsAreBillboarded());

  if (includeBB[1][2] != 0 && !data->parameters->getLabelsAreBillboarded())
    label->setPosition(Coord(coord[0], coord[1], coord[2] + size[2] / 2.));
  else
    label->setPosition(Coord(coord[0], coord[1], coord[2]));

  label->drawWithStencil(lod, camera);
}

void GlNode::getPointAndColor(GlGraphInputData *inputData, std::vector<Coord> &pointsCoordsArray,
                              std::vector<Color> &pointsColorsArray) {
  init(inputData);
  pointsCoordsArray.push_back(coord);

  node n = node(id);

  if (inputData->getElementBorderWidth()->getNodeValue(n) > 0) {
    pointsColorsArray.push_back(inputData->getElementBorderColor()->getNodeValue(n));
  } else {
    pointsColorsArray.push_back(inputData->getElementColor()->getNodeValue(n));
  }
}

void GlNode::getColor(GlGraphInputData *inputData, std::vector<Color> &pointsColorsArray) {
  node n = node(id);

  if (inputData->getElementBorderWidth()->getNodeValue(n) > 0) {
    pointsColorsArray.push_back(inputData->getElementBorderColor()->getNodeValue(n));
  } else {
    pointsColorsArray.push_back(inputData->getElementColor()->getNodeValue(n));
  }
}
}
