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
#include <tulip/GlDisplayListManager.h>
#include <tulip/OcclusionTest.h>
#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/GlSceneVisitor.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Camera.h>
#include <tulip/GlBezierCurve.h>
#include <tulip/GlCatmullRomCurve.h>
#include <tulip/GlOpenUniformCubicBSpline.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlVertexArrayManager.h>
#include <tulip/GlLabel.h>
#include <tulip/ParametricCurves.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlScene.h>
#include <tulip/GlGlyphRenderer.h>
#include <tulip/TulipViewSettings.h>


tlp::GlLabel* tlp::GlEdge::label=0;

using namespace std;

namespace tlp {

GlEdge::GlEdge(unsigned int id):id(id) {
  if(!label)
    label=new GlLabel();
}

BoundingBox GlEdge::getBoundingBox(const GlGraphInputData* data) {
  edge e = edge(id);
  BoundingBox bb;

  const std::pair<node, node>& eEnds = data->graph->ends(e);
  const node source = eEnds.first;
  const node target = eEnds.second;
  const Coord& srcCoord = data->getElementLayout()->getNodeValue(source);
  const Coord& tgtCoord = data->getElementLayout()->getNodeValue(target);

  const Size &srcSize = data->getElementSize()->getNodeValue(source);
  const Size &tgtSize = data->getElementSize()->getNodeValue(target);
  double srcRot = data->getElementRotation()->getNodeValue(source);
  double tgtRot = data->getElementRotation()->getNodeValue(target);

  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);

  // set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
  Coord srcAnchor, tgtAnchor, tmpAnchor;

  int srcGlyphId = data->getElementShape()->getNodeValue(source);
  Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
  tmpAnchor = (bends.size() > 0) ? bends.front() : tgtCoord;
  srcAnchor = sourceGlyph->getAnchor(srcCoord, tmpAnchor, srcSize, srcRot);

  int tgtGlyphId = 1; //cube outlined

  if (!data->getGraph()->isMetaNode(target))
    tgtGlyphId = data->getElementShape()->getNodeValue(target);

  Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
  //this time we don't take srcCoord but srcAnchor to be oriented to where the line comes from
  tmpAnchor = (bends.size() > 0) ? bends.back() : srcAnchor;
  tgtAnchor = targetGlyph->getAnchor(tgtCoord, tmpAnchor, tgtSize, tgtRot);

  if (!bends.empty()) {

    vector<Coord> tmp;
    tlp::computeCleanVertices(bends, srcCoord, tgtCoord, srcAnchor,tgtAnchor, tmp);

    for (vector<Coord>::iterator it = tmp.begin(); it != tmp.end(); ++it)
      bb.expand(*it);
  }

  bb.expand(srcAnchor);
  bb.expand(tgtAnchor);

  return bb;
}

void GlEdge::acceptVisitor(GlSceneVisitor *visitor) {
  visitor->visit(this);
}

void GlEdge::draw(float lod, const GlGraphInputData* data, Camera* camera) {

  edge e = edge(id);

  const std::pair<node, node>& eEnds = data->graph->ends(e);
  const node source = eEnds.first;
  const node target = eEnds.second;


  const Size &srcSize = data->getElementSize()->getNodeValue(source);
  const Size &tgtSize = data->getElementSize()->getNodeValue(target);

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

  getEdgeSize(data,e,srcSize,tgtSize,maxSrcSize,maxTgtSize,edgeSize);

  float lodSize = getEdgeWidthLod(data->getElementLayout()->getNodeValue(source),edgeSize,camera);

  bool selected = data->getElementSelected()->getEdgeValue(e);

  if (lod < 5) {
    if(data->getGlVertexArrayManager()->renderingIsBegin()) {
      data->getGlVertexArrayManager()->activatePointEdgeDisplay(this,selected);
    }
    else {
      const Coord& srcCoord = data->getElementLayout()->getNodeValue(source);
      Color srcCol, tgtCol;
      getEdgeColor(data,e,source,target,selected,srcCol,tgtCol);

      setColor(srcCol);
      glPointSize(1);
      glBegin(GL_POINTS);
      glVertex3f(srcCoord[0], srcCoord[1], srcCoord[2]);
      glEnd();

    }

    return;
  }

  std::string edgeTexture = data->getElementTexture()->getEdgeValue(e);

  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  unsigned nbBends = bends.size();

  bool vertexArrayRendering = false;

  if (data->getGlVertexArrayManager()->renderingIsBegin() &&
      !data->parameters->getFeedbackRender()) {
    if(lodSize>-5 && lodSize<5) {
      vertexArrayRendering = true;
      data->getGlVertexArrayManager()->activateLineEdgeDisplay(this,selected);
      return;
    }
    else if (!data->parameters->isEdge3D() && edgeTexture == "") {
      vertexArrayRendering = true;
      data->getGlVertexArrayManager()->activateQuadEdgeDisplay(this,selected);
    }
  }

  const Coord& srcCoord = data->getElementLayout()->getNodeValue(source);

  const Coord& tgtCoord = data->getElementLayout()->getNodeValue(target);

  if (selected) {
    glStencilFunc(GL_LEQUAL, data->parameters->getSelectedEdgesStencil(), 0xFFFF);
  }
  else {
    glStencilFunc(GL_LEQUAL, data->parameters->getEdgesStencil(), 0xFFFF);
  }

  glEnable(GL_COLOR_MATERIAL);

  const Color& fillColor = data->getElementColor()->getEdgeValue(e);
  const Color& strokeColor = data->getElementBorderColor()->getEdgeValue(e);
  const Color& textColor = data->getElementLabelColor()->getEdgeValue(e);

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

    glPassThrough(TLP_FB_BEGIN_EDGE);
    glPassThrough(static_cast<float>(id)); //id of the node for the feed back mode
  }

  Color srcCol, tgtCol;
  getEdgeColor(data,e,source,target,selected,srcCol,tgtCol);

  if (nbBends == 0 && (source == target)) { //a loop without bends
    //TODO : draw a nice loop!!
    return;
  }

  if (bends.size() == 0 && (srcCoord - tgtCoord).norm() < 1E-4)
    return; //two nodes very closed

  // set srcAnchor, tgtAnchor. tmpAnchor will be on the point just before tgtAnchor
  Coord srcAnchor, tgtAnchor, beginLineAnchor, endLineAnchor;

  getEdgeAnchor(data,source,target,bends,srcCoord,tgtCoord,srcSize,tgtSize,srcAnchor,tgtAnchor);

  if (data->parameters->isViewArrow()) {
    EdgeExtremityGlyph *startEdgeGlyph = data->extremityGlyphs.get(data->getElementSrcAnchorShape()->getEdgeValue(e));
    EdgeExtremityGlyph *endEdgeGlyph = data->extremityGlyphs.get(data->getElementTgtAnchorShape()->getEdgeValue(e));

    float selectionOutlineSize= 0.f;

    if (selected) {
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();

      Coord p1=camera->screenTo3DWorld(Coord(0,0,0));
      Coord p2=camera->screenTo3DWorld(Coord(2,0,0));
      selectionOutlineSize = (p2-p1).norm();
      edgeSize[0] += selectionOutlineSize;
      edgeSize[1] += selectionOutlineSize;

      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }

    if (startEdgeGlyph != NULL) {
      displayArrowAndAdjustAnchor(data,e,source,data->getElementSrcAnchorSize()->getEdgeValue(e),std::min(srcSize[0], srcSize[1]),srcCol,maxSrcSize,selected,selectionOutlineSize, endEdgeGlyph ? endEdgeGlyph->id() : UINT_MAX,
                                  bends.size(),(nbBends > 0) ? bends.front() : tgtCoord,tgtCoord,srcAnchor,tgtAnchor,beginLineAnchor, startEdgeGlyph, camera);
    }
    else {
      beginLineAnchor = srcAnchor;
    }

    if (endEdgeGlyph != NULL) {
      displayArrowAndAdjustAnchor(data,e,target,data->getElementTgtAnchorSize()->getEdgeValue(e),std::min(tgtSize[0], tgtSize[1]),tgtCol,maxTgtSize,selected,selectionOutlineSize,startEdgeGlyph ? startEdgeGlyph->id() : UINT_MAX,
                                  bends.size(),(nbBends > 0) ? bends.back() : srcAnchor,srcCoord,tgtAnchor,srcAnchor,endLineAnchor, endEdgeGlyph, camera);
    }
    else {
      endLineAnchor = tgtAnchor;
    }
  }
  else {
    beginLineAnchor = srcAnchor;
    endLineAnchor = tgtAnchor;
  }

  if (vertexArrayRendering)
    return;

  double lineWidth=data->getElementBorderWidth()->getEdgeValue(e);

  //Reset in case of drawing extremity glyph that can alterate value
  GlTextureManager::getInst().setAnimationFrame(data->getElementAnimationFrame()->getEdgeValue(e));
  //draw Edge
  drawEdge(srcCoord, tgtCoord, beginLineAnchor, endLineAnchor, bends, srcCol, tgtCol,camera->getCenter()-camera->getEyes(),data->parameters->isEdgeColorInterpolate() ,strokeColor,edgeSize,
           data->getElementShape()->getEdgeValue(e), data->parameters->isEdge3D(), lodSize, edgeTexture, static_cast<float>(lineWidth));
  GlTextureManager::getInst().setAnimationFrame(0);

  if (data->parameters->getFeedbackRender()) {
    glPassThrough(TLP_FB_END_EDGE);
  }

  glEnable(GL_LIGHTING);

}

#define L3D_BIT (1<<9)
void GlEdge::drawEdge(const Coord &srcNodePos, const Coord &tgtNodePos, const Coord &startPoint,
                      const Coord &endPoint, const LineType::RealType &bends, const Color &startColor,
                      const Color &endColor, const Coord &lookDir, bool colorInterpolate, const Color &borderColor,
                      const Size &size, int shape, bool edge3D, float lod,const string &textureName, const float outlineWidth) {

  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);

  if(bends.empty())
    shape=EdgeShape::Polyline;

  Coord srcDir(srcNodePos);
  Coord tgtDir(tgtNodePos);
  vector<Coord> tmp;
  computeCleanVertices(bends, startPoint, endPoint, srcDir, tgtDir,tmp);

  if (tmp.size() < 2) {
    return;
  }

  if (edge3D) {
    shape |= L3D_BIT;
    glEnable(GL_LIGHTING);
  }
  else {
    glDisable(GL_LIGHTING);
  }

  switch (shape) {
  case EdgeShape::Polyline:

    if (lod > 1000 || lod < -1000) {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,colorInterpolate,borderColor,textureName, outlineWidth);
    }
    else {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,true,borderColor,textureName, outlineWidth);
    }

    break;

  case L3D_BIT + EdgeShape::Polyline: {
    glDisable(GL_LIGHTING);
    simpleQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,lookDir,colorInterpolate,borderColor,textureName);
    glEnable(GL_LIGHTING);
    break;
  }

  case EdgeShape::BezierCurve :
  case EdgeShape::CatmullRomCurve :
  case EdgeShape::CubicBSplineCurve :
  case EdgeShape::BezierCurve + L3D_BIT:
  case EdgeShape::CatmullRomCurve + L3D_BIT:
  case EdgeShape::CubicBSplineCurve + L3D_BIT: {
    static GlBezierCurve bezier;
    static GlCatmullRomCurve catmull;
    static GlOpenUniformCubicBSpline bspline;
    AbstractGlCurve *curve = NULL;
    unsigned int nbCurvePoints = 200;

    if (shape == EdgeShape::BezierCurve || shape == EdgeShape::BezierCurve + L3D_BIT) {
      curve = &bezier;
    }
    else if (shape == EdgeShape::CatmullRomCurve || shape == EdgeShape::CatmullRomCurve + L3D_BIT) {
      curve = &catmull;
    }
    else {
      curve = &bspline;
    }

    bool fisheyeActivated = GlShaderProgram::getCurrentActiveShader() && GlShaderProgram::getCurrentActiveShader()->getName() == "fisheye";

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

    float startSize = size[0]*0.5f, endSize = size[1]*0.5f;

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
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,colorInterpolate,borderColor);
    }
    else {
      tlp::polyQuad(tmp, startColor, endColor, size[0] * .5f, size[1] * .5f, srcDir, tgtDir,true,borderColor);
    }

    break;
  }

  glDepthFunc(GL_LEQUAL);
}

void GlEdge::drawLabel(bool drawSelect,OcclusionTest* test, const GlGraphInputData* data,float lod) {
  edge e = edge(id);
  bool select = data->getElementSelected()->getEdgeValue(e);

  if(select!=drawSelect)
    return;

  drawLabel(test, data, lod);
}

void GlEdge::drawLabel(OcclusionTest* test, const GlGraphInputData* data) {
  drawLabel(test,data,0.);
}

void GlEdge::drawLabel(OcclusionTest* test, const GlGraphInputData* data, float lod, Camera *camera) {

  edge e = edge(id);

  const string &tmp = data->getElementLabel()->getEdgeValue(e);

  if (tmp.length() < 1) {
    return;
  }

  bool select = data->getElementSelected()->getEdgeValue(e);

  Color fontColor, outlineColor;

  if (data->getElementSelected()->getEdgeValue(e)) {
    fontColor = outlineColor = data->parameters->getSelectionColor();
  }
  else {
    fontColor = data->getElementLabelColor()->getEdgeValue(e);
    outlineColor = data->getElementLabelBorderColor()->getEdgeValue(e);
  }

  float outlineWidth = data->getElementLabelBorderWidth()->getEdgeValue(e);

  if(fontColor.getA()==0 && (outlineColor.getA()==0 || outlineWidth==0))
    return;

  int fontSize=data->getElementFontSize()->getEdgeValue(e);

  if(select)
    fontSize+=2;

  if(select)
    label->setStencil(data->parameters->getSelectedEdgesStencil());
  else
    label->setStencil(data->parameters->getEdgesLabelStencil());

  label->setFontNameSizeAndColor(data->getElementFont()->getEdgeValue(e),fontSize,fontColor);
  label->setText(tmp);
  label->setOutlineColor(outlineColor);
  label->setOutlineSize(outlineWidth);

  const std::pair<node, node>& eEnds = data->graph->ends(e);
  const node source = eEnds.first;
  const node target = eEnds.second;

  const Size &srcSize = data->getElementSize()->getNodeValue(source);
  const Size &tgtSize = data->getElementSize()->getNodeValue(target);
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

  getEdgeSize(data,e,srcSize,tgtSize,maxSrcSize,maxTgtSize,edgeSize);

  label->setTranslationAfterRotation(Coord());

  const Coord & srcCoord = data->getElementLayout()->getNodeValue(source);
  const Coord & tgtCoord = data->getElementLayout()->getNodeValue(target);
  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  Coord position;
  float angle;

  if (bends.empty()) {
    position = (srcCoord + tgtCoord) / 2.f;
    angle=atan((tgtCoord[1]-srcCoord[1])/(tgtCoord[0]-srcCoord[0]))*static_cast<float>(180./M_PI);
  }
  else {
    if (bends.size() % 2 == 0) {
      position = (bends[bends.size() / 2 - 1] + bends[bends.size() / 2]) / 2.f;
      angle=atan((bends[bends.size() / 2][1]-bends[bends.size() / 2 - 1][1])/(bends[bends.size() / 2][0]-bends[bends.size() / 2 - 1][0]))*static_cast<float>(180./M_PI);
    }
    else {
      position = bends[bends.size() / 2];
      Coord firstVector;
      Coord secondVector;

      if(bends.size()>1) {
        firstVector=bends[bends.size() / 2]-bends[bends.size() / 2 - 1];
        secondVector=bends[bends.size() / 2]-bends[bends.size() / 2+1];
      }
      else {
        firstVector=bends[bends.size() / 2]-srcCoord;
        secondVector=bends[bends.size() / 2]-tgtCoord;
      }

      float firstAngle=atan(firstVector[1]/firstVector[0])*static_cast<float>(180./M_PI);
      float secondAngle=atan(secondVector[1]/secondVector[0])*static_cast<float>(180./M_PI);

      Coord textDirection=firstVector+secondVector;

      if(textDirection[1]<0)
        label->setTranslationAfterRotation(Coord(0,-label->getTranslationAfterRotation()[1],0));

      angle=(firstAngle+secondAngle)/2.f;

      if(firstVector[0] *secondVector[0]>=0)
        angle+=90;

      if(angle>=90)
        angle=-180+angle;

    }
  }

  BoundingBox bb=getBoundingBox(data);
  int labelPos = data->getElementLabelPosition()->getEdgeValue(e);

  label->setSize(Size());
  label->rotate(0,0,angle);
  label->setAlignment(labelPos);
  label->setScaleToSize(false);
  label->setLabelsDensity(data->parameters->getLabelsDensity());

  if(!(data->parameters->getLabelsDensity()==100)) //labels overlap
    label->setOcclusionTester(test);
  else
    label->setOcclusionTester(NULL);

  label->setPosition(position);

  if (edgeSize[0] > edgeSize[1]) {
    label->setTranslationAfterRotation(Coord(0, -edgeSize[0]/2));
  }
  else {
    label->setTranslationAfterRotation(Coord(0, -edgeSize[1]/2));
  }

  label->setUseLODOptimisation(true,bb);
  label->setUseMinMaxSize(!data->parameters->isLabelFixedFontSize());
  label->setMinSize(data->parameters->getMinSizeOfLabel());
  label->setMaxSize(data->parameters->getMaxSizeOfLabel());
  label->setBillboarded(data->parameters->getLabelsAreBillboarded());

  label->drawWithStencil(lod,camera);
}

void GlEdge::getVertices(const GlGraphInputData *data,
                         std::vector<Coord> &linesCoordsArray) {
  edge e = edge(id);

  const std::pair<node, node>& eEnds = data->graph->ends(e);
  const node source = eEnds.first;
  const node target = eEnds.second;
  const Coord& srcCoord = data->getElementLayout()->getNodeValue(source);
  const Coord& tgtCoord = data->getElementLayout()->getNodeValue(target);
  bool selected = data->getElementSelected()->getEdgeValue(e);

  const LineType::RealType &bends = data->getElementLayout()->getEdgeValue(e);
  unsigned nbBends = bends.size();

  if (nbBends == 0 && (source == target)) { //a loop without bends
    return;
  }

  if (bends.size() == 0 && (srcCoord - tgtCoord).norm() < 1E-4)
    return;

  const Size &srcSize = data->getElementSize()->getNodeValue(source);
  const Size &tgtSize = data->getElementSize()->getNodeValue(target);

  float maxSrcSize, maxTgtSize;

  if(srcSize[0]>=srcSize[1])
    maxSrcSize=srcSize[0];
  else
    maxSrcSize=srcSize[1];

  if(tgtSize[0]>=tgtSize[1])
    maxTgtSize=tgtSize[0];
  else
    maxTgtSize=tgtSize[1];

  Coord srcAnchor, tgtAnchor;
  getEdgeAnchor(data,source,target,bends,srcCoord,tgtCoord,srcSize,tgtSize,srcAnchor,tgtAnchor);

  vector<Coord> vertices;
  Coord srcCoordTmp=srcCoord;
  Coord tgtCoordTmp=tgtCoord;

  EdgeExtremityGlyph *startEdgeGlyph = data->extremityGlyphs.get(data->getElementSrcAnchorShape()->getEdgeValue(e));
  EdgeExtremityGlyph *endEdgeGlyph = data->extremityGlyphs.get(data->getElementTgtAnchorShape()->getEdgeValue(e));


  Coord beginLineAnchor;

  if (data->parameters->isViewArrow() && startEdgeGlyph != NULL) {
    displayArrowAndAdjustAnchor(data,e,source,data->getElementSrcAnchorSize()->getEdgeValue(e),std::min(srcSize[0], srcSize[1]),Color(),maxSrcSize,selected,0, endEdgeGlyph ? endEdgeGlyph->id() : UINT_MAX,
                                bends.size(),(nbBends > 0) ? bends.front() : tgtCoord,tgtCoord,srcAnchor,tgtAnchor,beginLineAnchor);
  }
  else {
    beginLineAnchor = srcAnchor;
  }

  Coord endLineAnchor;

  if (data->parameters->isViewArrow() && endEdgeGlyph != NULL) {
    displayArrowAndAdjustAnchor(data,e,target,data->getElementTgtAnchorSize()->getEdgeValue(e),std::min(tgtSize[0], tgtSize[1]),Color(),maxTgtSize,selected,0,startEdgeGlyph ? startEdgeGlyph->id() : UINT_MAX,
                                bends.size(),(nbBends > 0) ? bends.back() : srcAnchor,srcCoord,tgtAnchor,srcAnchor,endLineAnchor);
  }
  else {
    endLineAnchor = tgtAnchor;
  }

  computeCleanVertices(bends, beginLineAnchor, endLineAnchor, srcCoordTmp, tgtCoordTmp, vertices);

  if(vertices.empty())
    return ;

  if ((vertices.size() > 2 && data->getElementShape()->getEdgeValue(e) == EdgeShape::BezierCurve) ||
      (vertices.size() == 3 && data->getElementShape()->getEdgeValue(e) == EdgeShape::CubicBSplineCurve)) {
    vector<Coord> curvePoints;
    computeBezierPoints(vertices, curvePoints, 200);
    vertices = curvePoints;
  }
  else if (vertices.size() > 2 && data->getElementShape()->getEdgeValue(e) == EdgeShape::CatmullRomCurve) {
    vector<Coord> curvePoints;
    computeCatmullRomPoints(vertices, curvePoints, false, 200);
    vertices = curvePoints;
  }

  if (vertices.size() > 2 && data->getElementShape()->getEdgeValue(e) == EdgeShape::CubicBSplineCurve) {
    vector<Coord> curvePoints;
    computeOpenUniformBsplinePoints(vertices, curvePoints, 3, 200);
    vertices = curvePoints;
  }


  size_t numberOfVertices=vertices.size();

  for(size_t i=0; i<numberOfVertices; ++i) {
    linesCoordsArray.push_back(vertices[i]);
  }
}

void GlEdge::getColors(const GlGraphInputData *data,
                       const Coord *vertices,unsigned int numberOfVertices,
                       std::vector<Color> &linesColorsArray) {

  edge e = edge(id);

  const std::pair<node, node>& eEnds = data->graph->ends(e);
  const node source = eEnds.first;
  const node target = eEnds.second;

  Color srcCol, tgtCol;

  if (data->parameters->isEdgeColorInterpolate()) {
    srcCol = data->getElementColor()->getNodeValue(source);
    tgtCol = data->getElementColor()->getNodeValue(target);
  }
  else {
    srcCol = tgtCol = data->getElementColor()->getEdgeValue(e);
  }

  vector<Color> colors;
  tlp::getColors(vertices,numberOfVertices,srcCol,tgtCol,colors);

  size_t numberOfColors=colors.size();

  for(size_t i=0; i<numberOfColors; ++i) {
    linesColorsArray.push_back(colors[i]);
  }
}

void GlEdge::getColors(const GlGraphInputData *data,
                       const vector<Coord> &vertices,
                       std::vector<Color> &linesColorsArray) {

  getColors(data,&vertices[0],vertices.size(),linesColorsArray);
}


void GlEdge::getEdgeColor(const GlGraphInputData *data,const edge &e,const node &source,const node &target, bool selected,Color &srcCol, Color &tgtCol) {
  Color selectionColor=data->parameters->getSelectionColor();

  if (selected) {
    srcCol = selectionColor;
    tgtCol = selectionColor;
  }
  else {
    if (data->parameters->isEdgeColorInterpolate()) {
      srcCol = data->getElementColor()->getNodeValue(source);
      tgtCol = data->getElementColor()->getNodeValue(target);
    }
    else {
      srcCol = tgtCol = data->getElementColor()->getEdgeValue(e);
    }
  }
}

void GlEdge::getEdgeSize(const GlGraphInputData *data,edge e,const Size &srcSize, const Size &tgtSize,const float maxSrcSize,const float maxTgtSize,Size &edgeSize) {

  if (data->parameters->isEdgeSizeInterpolate()) {
    if(srcSize[0]<srcSize[1])
      edgeSize[0]=srcSize[0]/8.f;
    else
      edgeSize[0]=srcSize[1]/8.f;

    if(tgtSize[0]<tgtSize[1])
      edgeSize[1]=tgtSize[0]/8.f;
    else
      edgeSize[1]=tgtSize[1]/8.f;
  }
  else {
    const Size &size = data->getElementSize()->getEdgeValue(e);
    edgeSize[0]=size[0];
    edgeSize[1]=size[1];

    if(data->parameters->getEdgesMaxSizeToNodesSize()) {
      edgeSize[0] = std::min(maxSrcSize, size[0]);
      edgeSize[1] = std::min(maxTgtSize, size[1]);
    }

    edgeSize[0]=edgeSize[0]/2.f;
    edgeSize[1]=edgeSize[1]/2.f;
  }
}

void GlEdge::getEdgeAnchor(const GlGraphInputData *data,const node &source,const node &target,const LineType::RealType &bends,const Coord &srcCoord,const Coord &tgtCoord,const Size &srcSize,const Size &tgtSize, Coord &srcAnchor, Coord &tgtAnchor) {
  double srcRot = data->getElementRotation()->getNodeValue(source);
  double tgtRot = data->getElementRotation()->getNodeValue(target);

  //compute anchor, (clip line with the glyph)
  int srcGlyphId = data->getElementShape()->getNodeValue(source);
  Glyph *sourceGlyph = data->glyphs.get(srcGlyphId);
  srcAnchor = (bends.size() > 0) ? bends.front() : tgtCoord;
  srcAnchor = sourceGlyph->getAnchor(srcCoord, srcAnchor, srcSize, srcRot);

  //compute anchor, (clip line with the glyph)
  int tgtGlyphId = data->getElementShape()->getNodeValue(target);
  Glyph *targetGlyph = data->glyphs.get(tgtGlyphId);
  tgtAnchor = (bends.size() > 0) ? bends.back() : srcAnchor;
  tgtAnchor = targetGlyph->getAnchor(tgtCoord, tgtAnchor, tgtSize, tgtRot);
}

float GlEdge::getEdgeWidthLod(const Coord &edgeCoord,
                              const Size &edgeSize,Camera *camera) {
  Matrix<float, 4u> projectionMatrix;
  Matrix<float, 4u> modelviewMatrix;
  camera->getProjectionMatrix(projectionMatrix);
  camera->getModelviewMatrix(modelviewMatrix);

  if (edgeSize[0] != edgeSize[1]) {
    return std::max(std::abs(projectSize(edgeCoord, Size(edgeSize[0], edgeSize[0], edgeSize[0]), projectionMatrix, modelviewMatrix,camera->getViewport())),
                    std::abs(projectSize(edgeCoord, Size(edgeSize[1], edgeSize[1], edgeSize[1]), projectionMatrix, modelviewMatrix,camera->getViewport())));
  }
  else {
    return std::abs(projectSize(edgeCoord, Size(edgeSize[0], edgeSize[0], edgeSize[0]), projectionMatrix, modelviewMatrix,camera->getViewport()));
  }
}

void GlEdge::displayArrowAndAdjustAnchor(const GlGraphInputData *data,
    const edge &e,
    const node &source,
    const Size& sizeRatio,
    float edgeSize,
    const Color &color,
    float maxSize,
    bool selected,
    float selectionOutlineSize,
    int tgtEdgeGlyph,
    size_t numberOfBends,
    const Coord &anchor,
    const Coord &tgtCoord,
    const Coord &srcAnchor,
    const Coord &tgtAnchor,
    Coord &lineAnchor,
    EdgeExtremityGlyph* extremityGlyph,
    Camera *camera) {

  //Correct begin tmp Anchor
  Coord beginTmpAnchor = anchor;

  if (beginTmpAnchor == tgtCoord) {
    beginTmpAnchor = tgtAnchor;
  }

  lineAnchor = beginTmpAnchor - srcAnchor;
  float nrm = lineAnchor.norm();
  float maxGlyphSize;

  if (tgtEdgeGlyph != 0 && numberOfBends == 0)
    maxGlyphSize = nrm * .5f;
  else
    maxGlyphSize = nrm;

  Size size;

  if (data->parameters->isEdgeSizeInterpolate()) {
    size[0] = edgeSize / 4.0f;
    size[1] = edgeSize / 4.0f;
    size[2] = edgeSize / 4.0f;

  }
  else {
    size[0] = sizeRatio[0];
    size[1] = sizeRatio[1];
    size[2] = sizeRatio[2];

    if(data->parameters->getEdgesMaxSizeToNodesSize()) {
      size[0] = std::min(maxSize, sizeRatio[0]);
      size[1] = std::min(maxSize, sizeRatio[1]);
      size[2] = std::min(maxSize, sizeRatio[2]);
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

    float lod = projectSize(srcAnchor, size, projectionMatrix, modelviewMatrix,camera->getViewport());

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
      }

      Color borderColor = data->parameters->isEdgeColorInterpolate() ? color : data->getElementBorderColor()->getEdgeValue(e);

      if (data->getGlGlyphRenderer()->renderingHasStarted() && noShaderGlyphs.find(extremityGlyph->id()) == noShaderGlyphs.end()) {
        data->getGlGlyphRenderer()->addEdgeExtremityGlyphRendering(extremityGlyph, e, source, color, borderColor, 100., beginTmpAnchor, srcAnchor, size, selected);
      }
      else {
        MatrixGL srcTransformationMatrix;
        MatrixGL srcScalingMatrix;

        extremityGlyph->get2DTransformationMatrix(beginTmpAnchor, srcAnchor, size,
            srcTransformationMatrix, srcScalingMatrix);

        glPushMatrix();
        glMultMatrixf((GLfloat *) &srcTransformationMatrix);
        glMultMatrixf((GLfloat *) &srcScalingMatrix);
        glDisable(GL_CULL_FACE);
        extremityGlyph->draw(e, source, color, borderColor, 100.);
        glEnable(GL_CULL_FACE);
        glPopMatrix();
      }
    }
  }

  //Compute new Anchor

  if (nrm > 0.00000001f) {
    lineAnchor /= nrm;
    lineAnchor *= size[0];
    lineAnchor += srcAnchor;
  }
  else {
    lineAnchor = srcAnchor;
  }
}

}
